#pragma once
#include "Connection.h"
#include "proto/mess_wl.pb.h"
#include "Buffer.h"
#include <queue>

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

	int download(int id, std::string& url);
	void setCallback(const std::shared_ptr<StateCallback> callback);
	void startStateLoop();

private:
	//decode one message a time
	int decode(downloadmessage::Download_Response* res);
	void dispatchMessage(const downloadmessage::Download_Response& res);

private:
	std::queue<downloadmessage::Mess_WL> downloadRequests_;
	std::shared_ptr<StateCallback> callback_;
	buffer inputBuf_;
};

class SimpleStateCallback 
	: public StateConnection::StateCallback
	, public std::enable_shared_from_this<SimpleStateCallback>
{
	// Inherited via StateCallback
	virtual void update(int id, const downloadmessage::Download_Response& res) override;
	virtual void paused(int id) override;
	virtual void resumed(int id) override;
	virtual void removed(int id) override;
	virtual void finished(int id) override;
};

