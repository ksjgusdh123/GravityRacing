#pragma once
#include "ISystem.h"

struct OperationContext
{
    SOCKET  ClientSocket;
    bool    bIsReceive;
};

class FRIONetworkSystem : public ISystem
{
public:
    void Init() override;
    void Shutdown() override;

    void Run();

private:
    void InitListenSocket();
    bool LoadRIOFunctions();
    void RegisterRIOCompletion();
    void AcceptLoop();
    void WorkerLoop();

private:
    SOCKET              ListenSocket    = INVALID_SOCKET;
    RIO_EXTENSION_FUNCTION_TABLE Rio     = {};
    RIO_CQ               CompletionQueue = RIO_INVALID_CQ;
    RIO_BUFFERID         BufferId        = RIO_INVALID_BUFFERID;

    char*                RecvBuffer      = nullptr;
    static constexpr ULONG RecvBufferSize = 8192;

    std::unordered_map<SOCKET, RIO_RQ> ClientRQMap;
    std::mutex                        ClientMutex;
};
