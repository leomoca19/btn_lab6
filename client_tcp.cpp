#include <iostream>
#include "socket.h"
using std::cout, std::cin, std::string, std::getline;
int main(){
    Socket client(false, SOCK_STREAM);

    cout << "Enter file name to request: ";
    string fname;
    getline(cin, fname);

    client.requestFile(fname);
    client.receiveFile("received_" + fname);

    return 0;
}
