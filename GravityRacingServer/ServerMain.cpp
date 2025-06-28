#include "PCH.h"
#include "FGameServer.h"

int main()
{
    try {
        FGameServer Server;
        if (!Server.Start()) {
            std::cerr << "서버 실행 실패\n";
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "예외 발생: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}