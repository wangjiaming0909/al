#pragma once
#include "Connection.h"
#include "proto/mess_wl.pb.h"
#include "Buffer.h"

class StateConnection : public Connection
{
public:
	struct StateCallback
	{
		virtual void update(int id, const downloadmessage::Download_Response& res) = 0;
		virtual void paused(int id) = 0;
		virtual void resumed(int id) = 0;
		virtual void removed(int id) = 0;
		virtual void finished(int id) = 0;
	};
	StateConnection(const std::string& addrStr, uint32_t port);

	void setCallback(const std::shared_ptr<StateCallback> callback);
	void setCallback(StateCallback* callback);
	void startStateLoop();

private:
	//decode one message a time
	int decode(downloadmessage::Download_Response* res);
	void dispatchMessage(const downloadmessage::Download_Response& res);

private:
	StateCallback* callback_ = nullptr;
	buffer inputBuf_;
};

