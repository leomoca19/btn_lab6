#include "socket.h"
#include <iostream>
#include <sstream>

Socket::Socket(bool server, int proto) : isServer(server), protocol(proto) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    sock = socket(AF_INET, protocol, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    if (isServer) {
        addr.sin_addr.s_addr = INADDR_ANY;
        bind(sock, (SOCKADDR*)&addr, sizeof(addr));
        if (protocol == SOCK_STREAM) {
            listen(sock, SOMAXCONN);
        }
    } else {
        addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        if (protocol == SOCK_STREAM) {
            connect(sock, (SOCKADDR*)&addr, sizeof(addr));
        }
    }
}

Socket::~Socket() {
    closesocket(sock);
    WSACleanup();
}

Socket Socket::acceptConnection() {
    SOCKET clientSock = accept(sock, NULL, NULL);
    Socket clientSocket(false, SOCK_STREAM);
    clientSocket.sock = clientSock;
    return clientSocket;
}

void Socket::sendPDU(const std::string& pdu, const SOCKADDR_IN* destAddr) {
    if (protocol == SOCK_STREAM) {
        send(sock, pdu.c_str(), pdu.length(), 0);
    } else {
        if (destAddr) {
            sendto(sock, pdu.c_str(), pdu.length(), 0, (SOCKADDR*)destAddr, sizeof(*destAddr));
        }
    }
}

std::string Socket::receivePDU(SOCKADDR_IN* senderAddr) {
    char buffer[1024];
    int bytesReceived;
    if (protocol == SOCK_STREAM) {
        bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
    } else {
        if (senderAddr) {
            int senderAddrLen = sizeof(*senderAddr);
            bytesReceived = recvfrom(sock, buffer, sizeof(buffer), 0, (SOCKADDR*)senderAddr, &senderAddrLen);
        }
    }
    if (bytesReceived > 0) {
        return std::string(buffer, bytesReceived);
    }
    return "";
}

std::string Socket::createFilenamePDU(const std::string& filename) {
    return "F " + filename;
}

std::string Socket::createDataPDU(const std::string& data) {
    return "D " + data;
}

std::string Socket::createLastPDU(const std::string& data) {
    return "L " + data;
}

std::string Socket::createErrorPDU(const std::string& errorMessage) {
    return "E " + errorMessage;
}

std::pair<char, std::string> Socket::parsePDU(const std::string& pdu) {
    if (pdu.length() < 2) {
        return std::make_pair('?', "");
    }
    char type = pdu[0];
    std::string data = pdu.substr(2);
    return std::make_pair(type, data);
}

void Socket::sendFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        sendPDU(createErrorPDU("File not found"));
        return;
    }
    sendPDU(createDataPDU("FILE_FOUND"));
    char buffer[100];
    while (file) {
        file.read(buffer, sizeof(buffer));
        sendPDU(createDataPDU(std::string(buffer, file.gcount())));
    }
    file.close();
    sendPDU(createLastPDU(""));
}

void Socket::receiveFile(const std::string& filename) {
    std::string pdu = receivePDU();
    std::pair<char, std::string> parsed = parsePDU(pdu);
    if (parsed.first == 'E') {
        std::cout << "Error: " << parsed.second << std::endl;
        return;
    }
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to create file." << std::endl;
        return;
    }
    while (true) {
        pdu = receivePDU();
        parsed = parsePDU(pdu);
        if (parsed.first == 'D') {
            file.write(parsed.second.c_str(), parsed.second.length());
        } else if (parsed.first == 'L') {
            break;
        }
    }
    file.close();
    std::cout << "File received successfully." << std::endl;
}

void Socket::requestFile(const std::string& filename) {
    sendPDU(createFilenamePDU(filename));
}
