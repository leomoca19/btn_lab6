#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include "utils.h"
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
#define PORT 8080

struct sockaddr_in servAddr;

int main(){
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (result != 0){
		panic("WSAStartup failed");
	}

	const size_t bf_sz = 1024;
	char buffer[bf_sz]{};

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKET client = INVALID_SOCKET;
	client = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == client) {
		panic("Error at socket()", &client);
	}

	result = connect(client, (struct sockaddr*)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == result || INVALID_SOCKET == client){
		panic("Unable to connect to server", &client);
	}

	// Connection established
	cout << recv_msg(&client) << endl;

	// Send connection ack
	send_msg(&client, "Hello from client");

	// Connection closed
	cout << recv_msg(&client) << endl;
	closesocket(client);
	WSACleanup();
	return 0;
}
