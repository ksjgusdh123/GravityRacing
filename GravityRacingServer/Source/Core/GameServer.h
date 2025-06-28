#pragma once
#include "SystemManager.h"

class FGameServer
{
public:
    FGameServer();
    ~FGameServer();
    
    bool Start();
private:
    bool Init();
    void Run();
    void Shutdown();

private:
    FSystemManager SystemMgr;
};