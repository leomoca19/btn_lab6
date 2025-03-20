#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "utils.h"
#pragma comment(lib, "Ws2_32.lib")
using namespace std;
#define PORT 8080

struct sockaddr_in serverAddr, clientAddr;

int main(){
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (res != 0){
		panic("WSAStartup failed");
	}

	int addrlen = sizeof(clientAddr);
	const size_t bf_sz = 1024;
	char buffer[bf_sz]{};

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
	
	res = bind(server, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (res != 0) {
		panic("Bind failed", &server);
	}

	if (listen(server, 5) == SOCKET_ERROR) {
		panic("Listen failed", &server);
	}
	cout << "Listening on port " << PORT << endl;

	SOCKET client = INVALID_SOCKET;
	client = accept(server, nullptr, nullptr);
	if ( INVALID_SOCKET == client) {
		panic("Accept failed", &server);
	}

	// main execution
	
	// Connection confirmation
	send_msg(&client, "Hello from server");
	// Connection acknowledgement
	cout << recv_msg(&client) << endl;

	send_msg(&client, "Bye from server");
	cout << "Server is closing" << endl;
	closesocket(client);
	closesocket(server);
	WSACleanup();
	
	return 0;
}

