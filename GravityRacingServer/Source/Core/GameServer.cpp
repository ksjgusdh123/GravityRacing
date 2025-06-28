#include "PCH.h"
#include "GameServer.h"

FGameServer::FGameServer()
{
}

FGameServer::~FGameServer()
{
}

bool FGameServer::Start()
{
    if (!Init()) {
        return false;
    }

    Run();
    Shutdown();
    return true;
}

bool FGameServer::Init()
{
    return false;
}

void FGameServer::Run()
{
}

void FGameServer::Shutdown()
{
}
