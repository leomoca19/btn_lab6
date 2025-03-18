#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

inline void panic(const char* str="something went wrong", SOCKET *sock=nullptr)  {
	std::cerr << str << ", error: " << WSAGetLastError() << std::endl;
	if (nullptr != sock) {
		closesocket(*sock);
		WSACleanup();
	}
	exit(1);
}

inline void send_msg(const SOCKET *sock, const char* str) {
	int result = send(*sock, str, strlen(str), 0);
	if (SOCKET_ERROR == result) {
		std::cerr << "Send failed, error: " << WSAGetLastError() << std::endl;
	}
}

inline std::string recv_msg(const SOCKET *sock) {
	const size_t sz = 1024;
	char buffer[sz]{};

	int result = recv(*sock, buffer, sz - 1, 0);
	if (SOCKET_ERROR == result) {
		std::cerr << "Receive failed, error: " << WSAGetLastError() << std::endl;
		return "";
	}

	return std::string(buffer, result);
}

#endif
