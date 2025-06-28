#include "PCH.h"
#include "Logger.h"
#include "LoggerSystem.h"

void FLoggerSystem::Init()
{
    FLogger::Init();
    FLogger::Info("Logger system initialized.");
}

void FLoggerSystem::Shutdown()
{
    FLogger::Info("Logger system shutdown.");
    spdlog::shutdown();
}
