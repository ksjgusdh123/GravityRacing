#include "PCH.h"
#include "GameServer.h"
#include "Network/RIONetworkSystem.h"

FGameServer::FGameServer()
{
    LOG("FGameServer constructed.");
}

FGameServer::~FGameServer()
{
    Shutdown();
    LOG("FGameServer destructed.");
}

bool FGameServer::Start()
{
    LOG("Starting server...");

    if (!Init())
    {
        LOGE("Server initialization failed.");
        return false;
    }

    Run();
    return true;
}

bool FGameServer::Init()
{
    LOG("Initializing server...");
    SystemMgr.RegisterSystem<FRIONetworkSystem>();

    SystemMgr.InitAll();
    return true;
}


void FGameServer::Run()
{
    LOG("Server is now running.");

    auto Network = SystemMgr.GetSystem<FRIONetworkSystem>();
    if (Network)
    {
        Network->Run();
    }
    else
    {
        LOGW("FRIONetworkSystem not found!");
    }
}

void FGameServer::Shutdown()
{
    LOG("Shutting down server...");

    SystemMgr.ShutdownAll();
}
