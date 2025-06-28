#include "PCH.h"
#include "Network/RIONetworkSystem.h"

void FRIONetworkSystem::Init()
{
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::runtime_error("WSAStartup failed");

    InitListenSocket();

    if (!LoadRIOFunctions())
        throw std::runtime_error("RIO function load failed");

    RegisterRIOCompletion();
}

void FRIONetworkSystem::Shutdown()
{
    LOG("Shutting down RIO Network System");

    if (BufferId != RIO_INVALID_BUFFERID)
    {
        Rio.RIODeregisterBuffer(BufferId);
        BufferId = RIO_INVALID_BUFFERID;
    }
    delete[] RecvBuffer;
    RecvBuffer = nullptr;

    {
        std::lock_guard lock(ClientMutex);
        for (auto& [sock, rq] : ClientRQMap)
            closesocket(sock);
        ClientRQMap.clear();
    }

    if (CompletionQueue != RIO_INVALID_CQ)
    {
        Rio.RIOCloseCompletionQueue(CompletionQueue);
        CompletionQueue = RIO_INVALID_CQ;
    }

    if (ListenSocket != INVALID_SOCKET)
    {
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
    }

    WSACleanup();
}

void FRIONetworkSystem::Run()
{
    LOG("Starting RIO accept & worker threads");

    std::thread acceptThread(&FRIONetworkSystem::AcceptLoop, this);
    std::thread workerThread(&FRIONetworkSystem::WorkerLoop, this);

    acceptThread.join();
    workerThread.join();
}

void FRIONetworkSystem::InitListenSocket()
{
    ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_REGISTERED_IO);
    if (ListenSocket == INVALID_SOCKET)
        throw std::runtime_error("WSASocket failed");

    SOCKADDR_IN addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(ListenSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == SOCKET_ERROR)
        throw std::runtime_error("bind failed");

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
        throw std::runtime_error("listen failed");

    LOG("Listen socket ready on port 7777");
}

bool FRIONetworkSystem::LoadRIOFunctions()
{
    GUID gid = WSAID_MULTIPLE_RIO;
    DWORD bytes = 0;
    return WSAIoctl(
        ListenSocket,
        SIO_GET_MULTIPLE_EXTENSION_FUNCTION_POINTER,
        &gid, sizeof(gid),
        &Rio, sizeof(Rio),
        &bytes,
        nullptr, nullptr
    ) == 0;
}

void FRIONetworkSystem::RegisterRIOCompletion()
{
    CompletionQueue = Rio.RIOCreateCompletionQueue(1024, nullptr);
    if (CompletionQueue == RIO_INVALID_CQ)
        throw std::runtime_error("RIOCreateCompletionQueue failed");

    RecvBuffer = new char[RecvBufferSize];
    BufferId = Rio.RIORegisterBuffer(RecvBuffer, RecvBufferSize);
    if (BufferId == RIO_INVALID_BUFFERID)
        throw std::runtime_error("RIORegisterBuffer failed");

    LOG("RIO CompletionQueue & Buffer initialized");
}

void FRIONetworkSystem::AcceptLoop()
{
    LOG("AcceptLoop started");

    while (true)
    {
        SOCKET client = accept(ListenSocket, nullptr, nullptr);
        if (client == INVALID_SOCKET)
        {
            LOGW("accept() failed: {}", WSAGetLastError());
            continue;
        }
        LOG("Client connected: {}", (uint64_t)client);

        RIO_RQ clientRQ = Rio.RIOCreateRequestQueue(
            client,
            1, 1,    // recv slots
            1, 1,    // send slots
            CompletionQueue, CompletionQueue,
            nullptr
        );
        if (clientRQ == RIO_INVALID_RQ)
        {
            LOGE("RIOCreateRequestQueue(client) failed: {}", WSAGetLastError());
            closesocket(client);
            continue;
        }

        {
            std::lock_guard lock(ClientMutex);
            ClientRQMap[client] = clientRQ;
        }

        OperationContext* recvCtx = new OperationContext{ client, true };
        RIO_BUF buf{ BufferId, 0, RecvBufferSize };
        if (!Rio.RIOReceive(clientRQ, &buf, 1, 0, recvCtx))
        {
            LOGE("RIOReceive initial failed");
            delete recvCtx;
            std::lock_guard lock(ClientMutex);
            ClientRQMap.erase(client);
            closesocket(client);
            continue;
        }

        LOG("Receive issued to client: {}", (uint64_t)client);
    }
}

void FRIONetworkSystem::WorkerLoop()
{
    LOG("WorkerLoop started");
    const ULONG maxResults = 16;
    std::vector<RIORESULT> results(maxResults);

    while (true)
    {
        ULONG num = Rio.RIODequeueCompletion(CompletionQueue, results.data(), maxResults);
        if (num == 0)
        {
            std::this_thread::yield();
            continue;
        }

        for (ULONG i = 0; i < num; ++i)
        {
            auto& r = results[i];
            auto* ctx = reinterpret_cast<OperationContext*>(r.RequestContext);
            SOCKET client = ctx->ClientSocket;

            if (r.BytesTransferred == 0)
            {
                LOG("Client disconnected: {}", (uint64_t)client);
                {
                    std::lock_guard lock(ClientMutex);
                    ClientRQMap.erase(client);
                }
                closesocket(client);
                continue;
            }

            if (ctx->bIsReceive)
            {
                LOG("Received {} bytes from {}", r.BytesTransferred, (uint64_t)client);

                {
                    std::lock_guard lock(ClientMutex);
                    RIO_RQ rq = ClientRQMap[client];
                    RIO_BUF sendBuf{ BufferId, 0, r.BytesTransferred };
                    auto* sendCtx = new OperationContext{ client, false };
                    if (!Rio.RIOSend(rq, &sendBuf, 1, 0, sendCtx))
                    {
                        LOGW("RIOSend failed");
                        delete sendCtx;
                    }
                }

                {
                    std::lock_guard lock(ClientMutex);
                    RIO_RQ rq = ClientRQMap[client];
                    auto* nextCtx = new OperationContext{ client, true };
                    RIO_BUF recvBuf{ BufferId, 0, RecvBufferSize };
                    if (!Rio.RIOReceive(rq, &recvBuf, 1, 0, nextCtx))
                    {
                        LOGW("RIOReceive reissue failed");
                        delete nextCtx;
                    }
                }
            }
            else
            {
                LOG("Send completed for {}", (uint64_t)client);
                delete ctx;
            }
        }
    }
}
