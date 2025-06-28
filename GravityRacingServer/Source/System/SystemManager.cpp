#include "PCH.h"
#include "SystemManager.h"

void ISystem::Init()
{
}

void ISystem::Shutdown()
{
}

void FSystemManager::Register(std::shared_ptr<ISystem> system)
{
}

void FSystemManager::InitAll()
{
}

void FSystemManager::ShutdownAll()
{
}
