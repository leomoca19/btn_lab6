#ifndef SOCKET_H
#define SOCKET_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <fstream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

class Socket {
private:
    SOCKET sock;
    SOCKADDR_IN addr;
    bool isServer;
    int protocol;

    std::string createFilenamePDU(const std::string& filename);
    std::string createDataPDU(const std::string& data);
    std::string createLastPDU(const std::string& data);
    std::string createErrorPDU(const std::string& errorMessage);

public:
    Socket(bool server, int proto);
    ~Socket();

    std::pair<char, std::string> parsePDU(const std::string& pdu);
    Socket acceptConnection();
    void sendPDU(const std::string& pdu, const SOCKADDR_IN* destAddr = nullptr);
    std::string receivePDU(SOCKADDR_IN* senderAddr = nullptr);
    void sendFile(const std::string& filename);
    void receiveFile(const std::string& filename);
    void requestFile(const std::string& filename);
};

#endif
