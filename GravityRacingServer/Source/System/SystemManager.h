#pragma once

class ISystem
{
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
};

class FSystemManager
{
public:
    static void Register(std::shared_ptr<ISystem> system);
    static void InitAll();
    static void ShutdownAll();

private:
    static std::vector<std::shared_ptr<ISystem>> Systems;
};