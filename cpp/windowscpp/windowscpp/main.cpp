#include <iostream>
#include "sockpp/connector.h"
#include "sockpp/inet_address.h"
#include "proto/mess_wl.pb.h"
#include <thread>
#include <vector>
#include "ConfigParser.h"
#include "StateConnection.h"
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
	std::string name = "pc";
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
	if (!connector.close())
	{
		cout << "close error..." << endl;
	}
}


int main(int argc, char** argv)
{
	//if (argc < 2)
	//{
	//	cout << "url" << endl;
	//	exit(-1);
	//}
	init();
	//std::string url = argv[1];

	//int i = 9;
	//while (i == 9)
	//{
	//	cout << "----------------------" << i << endl;
	//	std::vector<std::thread*> threads{};
	//	for (int i = 0; i < 1; i++)
	//	{
	//		threads.push_back(new std::thread(sendDownload, std::ref(url)));
	//	}

	//	for (int i = 0; i < 1; i++)
	//	{
	//		threads[i]->join();
	//		delete threads[i];
	//	}
	//	i--;
	//}

	auto option = config::ConfigParser::parse();
	cout << "host: " << option.targetHost << endl;
	cout << "port: " << option.port << std::endl;

	Connection conn{ "pc", 9090 };
	cout << conn.getAddr().to_string() << endl;

	Connection conn2{"192.168.0.2", 9090};
	cout << conn2.getAddr().to_string() << endl;

	try
	{
		auto addr = sockpp::inet_address::resolve_name("pc");
		auto addr2 = sockpp::inet_address::resolve_name("192.168.0.2");
		in_addr a;
		inet_pton(AF_INET, "192.168.0.2", &a);
		sockpp::inet_address ad(addr, 90);
		sockpp::inet_address ad2(addr2, 90);
		sockaddr_in sockaddr{};
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port = 90;
		sockaddr.sin_addr = a;
		sockpp::inet_address ad3(sockaddr);

		cout << ad.to_string() << endl;
		cout << ad2.to_string() << endl;
		cout << ad3.to_string() << endl;
	}
	catch (const std::exception & ex)
	{
		cout << ex.what() << endl;
	}
	return 0;
}