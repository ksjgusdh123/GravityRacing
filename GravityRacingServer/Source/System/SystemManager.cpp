#include "PCH.h"
#include "SystemManager.h"

FSystemManager::FSystemManager() = default;
FSystemManager::~FSystemManager() = default;

void FSystemManager::InitAll()
{
    for (auto& [type, system] : Systems)
        system->Init();
}

void FSystemManager::ShutdownAll()
{
    for (auto& [type, system] : Systems | std::views::reverse)
        system->Shutdown();
}
