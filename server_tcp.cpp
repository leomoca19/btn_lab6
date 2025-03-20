#include <iostream>
#include "socket.h"
using std::cout, std::string, std::endl, std::pair;
int main() {
    Socket server(true, SOCK_STREAM);
    Socket client = server.acceptConnection();

    while (true){
        string pdu = client.receivePDU();
        pair<char, string> parsed = client.parsePDU(pdu);

        if (parsed.first == 'F') {
            cout << "Received file request: " << parsed.second << endl;
            client.sendFile(parsed.second);
        }
        else if (parsed.first == '?') {
            cout << "Client dissconnected" << endl;
            break;
        }
    }
    return 0;
}
