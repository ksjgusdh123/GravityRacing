#include "PCH.h"
#include "GameServer.h"

int main()
{
    FLogger::Init();

    try {
        FGameServer Server;
        if (!Server.Start()) {
            LOGE("Server failed to start");
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& ex) {
        LOGE("{}", ex.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
