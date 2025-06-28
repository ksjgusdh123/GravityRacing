#pragma once
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

};

