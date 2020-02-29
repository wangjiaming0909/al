#pragma once
#include "Connection.h"
class DownloadRequestConnection : public Connection
{
public:
	DownloadRequestConnection(const std::string& addrStr, uint32_t port);

	int download(int id, const std::string& url);
	int pause(int id);
	int remove(int id);
	int resume(int id);
};

