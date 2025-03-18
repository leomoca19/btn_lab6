#ifndef UDP_HANDLE_H
#define UDP_HANDLE_H

#include <string>

enum PDUType {
	FILENAME,
	DATA,
	LAST,
	ERROR,
	DEFAULT
};

class PDU {
	PDUType type{PDUType::DEFAULT};
	std::string data{};
};
	
#endif
