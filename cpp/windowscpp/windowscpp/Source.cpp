#include <iostream>
#include "sockpp/connector.h"
#include "sockpp/inet_address.h"
using namespace std;

void init()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* Winsock DLL.                                  */
		printf("WSAStartup failed with error: %d\n", err);
		exit(1);
	}
}

int main()
{
	init();

	std::string name = "ubuntu";
	sockpp::inet_address target = sockpp::inet_address(name, 80);
	auto connector = sockpp::connector(target);
	return 0;
}