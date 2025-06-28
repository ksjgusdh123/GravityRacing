#include "PCH.h"
#include "GameServer.h"
#include "Logger.h"

int main()
{
	FLogger::Init();

	FLogger::Info("Init");
	FLogger::Warn("경고 테스트 메시지");
	FLogger::Error("치명적 오류 발생!");

//    try {
//        FGameServer Server;
//        if (!Server.Start()) {
//            std::cerr << "서버 실행 실패\n";
//            return EXIT_FAILURE;
//        }
//    }
//    catch (const std::exception& ex) {
//        std::cerr << "예외 발생: " << ex.what() << "\n";
//        return EXIT_FAILURE;
//    }
//
//    return EXIT_SUCCESS;

}