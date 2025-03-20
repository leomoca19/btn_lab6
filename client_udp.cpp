#include <iostream>
#include "socket.h"
using namespace std;
int main(){
    Socket client(false, SOCK_DGRAM);

    string fname;
    cout << "Enter a file name to request: ";
    getline(cin, fname);

    client.requestFile(fname);
    client.receiveFile("received_" + fname);
    cout << fname << " received" << endl;
    return 0;
}
