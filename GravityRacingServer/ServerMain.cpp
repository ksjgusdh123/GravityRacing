#include "PCH.h"
#include "FGameServer.h"

int main()
{
    try {
        FGameServer Server;
        if (!Server.Start()) {
            std::cerr << "���� ���� ����\n";
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "���� �߻�: " << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}