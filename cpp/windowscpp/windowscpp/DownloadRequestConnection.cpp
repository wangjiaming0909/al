#include "DownloadRequestConnection.h"
#include "proto/mess_wl.pb.h"

DownloadRequestConnection::DownloadRequestConnection(const std::string& addrStr, uint32_t port)
	: Connection(addrStr, port)
{
}

int DownloadRequestConnection::download(int id, const std::string& url)
{
	using namespace downloadmessage;
	Mess_WL mess{};
	mess.set_command(Mess_WL_DownloadCommand::Mess_WL_DownloadCommand_DOWNLOAD);
	mess.set_id(id);
	mess.set_url(url);
	auto len =
		mess.url().size() +
		sizeof(Mess_WL::DownloadCommand) +
		sizeof(mess.id()) +
		sizeof(mess.len());
	mess.set_len(len);
	char arr[4096] = {};
	mess.SerializeToArray(arr, 4096);
	if (write(arr, len) != len) 
	{
		std::cout << "write error..." << std::endl;
		return -1;
	}
	return 0;
}

int DownloadRequestConnection::pause(int id)
{
	return 0;
}

int DownloadRequestConnection::remove(int id)
{
	return 0;
}

int DownloadRequestConnection::resume(int id)
{
	return 0;
}
