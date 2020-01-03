#include <iostream>
#include "sockpp/connector.h"
#include "sockpp/inet_address.h"
#include "proto/mess_wl.pb.h"
#include <thread>
#include <vector>
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
void sendDownload(const std::string& url)
{
	std::string name = "ubuntu";
	sockpp::inet_address target = sockpp::inet_address(name, 8000);
	auto connector = sockpp::connector();
	downloadmessage::Mess_WL mess{};
	mess.set_id(1);
	mess.set_command(downloadmessage::Mess_WL::DownloadCommand::Mess_WL_DownloadCommand_DOWNLOAD);
	mess.set_url(url);
	cout << mess.ByteSizeLong() << endl;
	auto len =
		mess.url().size() +
		sizeof(downloadmessage::Mess_WL::DownloadCommand) +
		sizeof(mess.id()) +
		sizeof(mess.len());
	mess.set_len(len);

	cout << len << endl;

	auto ret = connector.connect(target);
	if (!ret)
	{
		connector.close();
		cout << "connector error.." << endl;
		exit(-1);
	}
	char* d = (char*)::calloc(1024, 1);
	mess.SerializeToArray(d, 1024);
	auto sizeSend = connector.write(d, len);
	if (sizeSend != len)
	{
		cout << "error..." << endl;
	}
	free(d);
	connector.close();
}


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "url" << endl;
		exit(-1);
	}
	init();
	std::string url = argv[1];

	std::vector<std::thread*> threads{};
	for (int i = 0; i < 10; i++)
	{
		threads.push_back(new std::thread(sendDownload, std::ref(url)));
	}

	for (int i = 0; i < 10; i++)
	{
		threads[i]->join();
		delete threads[i];
	}

	return 0;
}