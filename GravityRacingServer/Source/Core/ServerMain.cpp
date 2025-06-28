#include "PCH.h"
#include "GameServer.h"
#include "Logger/LoggerSystem.h"
#include "Logger/Logger.h"

int main()
{
    FLoggerSystem::Init();
    try {
        FGameServer Server;
        if (!Server.Start()) {
            FLogger::Error("Server failed to start.");
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& ex) {
        FLogger::Error("Exception caught in main: {}", ex.what());
        return EXIT_FAILURE;
    }

    FLogger::Info("Server shutdown normally.");
    return EXIT_SUCCESS;
}
