#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "WSAStartup failed\n";
		return 1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET) {
		std::cerr << "socket() failed: " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7777);
	InetPtonA(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "connect() failed: " << WSAGetLastError() << "\n";
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	std::cout << "Connected to server. (type 'exit' to quit)\n";

	std::string msg;
	constexpr int BUF_SIZE = 4096;
	char buffer[BUF_SIZE];

	while (true)
	{
		std::cout << "> ";
		if (!std::getline(std::cin, msg))
			break;
		if (msg == "exit")
			break;
		int sent = send(sock, msg.c_str(), (int)msg.size(), 0);
		if (sent == SOCKET_ERROR) {
			std::cerr << "send() failed: " << WSAGetLastError() << "\n";
			break;
		}

		int received = recv(sock, buffer, BUF_SIZE - 1, 0);
		if (received > 0) {
			buffer[received] = '\0';
			std::cout << "< " << buffer << "\n";
		}
		else if (received == 0) {
			std::cout << "Server closed connection.\n";
			break;
		}
		else {
			std::cerr << "recv() failed: " << WSAGetLastError() << "\n";
			break;
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}
