#include <iostream>
#include "socket.h"
using namespace std;
int main(){
    Socket server(true, SOCK_DGRAM);

    while (true) {
        SOCKADDR_IN client;
        string pdu = server.receivePDU(&client);
        pair<char, string> parsed = server.parsePDU(pdu);

        if (parsed.first == 'F') {
            cout << "Received file request: " << parsed.second << endl;
            server.sendFile(parsed.second);
        }
    }
    return 0;
}
