#include <iostream>
#include "sockpp/connector.h"
#include "sockpp/inet_address.h"
#include "proto/mess_wl.pb.h"
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
	downloadmessage::Mess_WL mess{};
	mess.set_id(1);
	mess.set_command(downloadmessage::Mess_WL::DownloadCommand::Mess_WL_DownloadCommand_DOWNLOAD);
	mess.set_url("");
	mess.set_len(mess.ByteSizeLong());
	cout << mess.ByteSizeLong() << endl;
	cout << 
		mess.url().size() + 
		sizeof(downloadmessage::Mess_WL::DownloadCommand) + 
		sizeof(mess.id()) + 
		sizeof(mess.len()) << endl;
	return 0;
}