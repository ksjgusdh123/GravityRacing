#include "PCH.h"
#include "Logger.h"
#include "LoggerSystem.h"

void FLoggerSystem::Init()
{
    FLogger::Init();
}

void FLoggerSystem::Shutdown()
{
    spdlog::shutdown();
}
