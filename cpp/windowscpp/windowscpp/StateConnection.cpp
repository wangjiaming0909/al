#include "StateConnection.h"

StateConnection::StateConnection(const std::string& addrStr, uint32_t port)
	: Connection(addrStr, port), inputBuf_()
{
}

void StateConnection::setCallback(const std::shared_ptr<StateCallback> callback)
{
	std::weak_ptr<StateCallback> ptr = callback;
}

void StateConnection::setCallback(StateCallback* callback)
{
	callback_ = callback;
}

void StateConnection::startStateLoop()
{
	while (this->connector_->is_open())
	{
		char d[4096] = {};
		auto sizeRead = read(d, 4096);
		downloadmessage::Download_Response res;
		inputBuf_.append(d, sizeRead);
		while (inputBuf_.total_len() > sizeof(downloadmessage::Download_Response))
		{
			decode(&res);
			dispatchMessage(res);
			res.Clear();
		}
	}
}

int StateConnection::decode(downloadmessage::Download_Response* res)
{
	using namespace downloadmessage;
	auto sizeGoingToParse = sizeof(Download_Response);
	assert(inputBuf_.total_len() >= sizeGoingToParse);
	auto d = inputBuf_.pullup(sizeGoingToParse);
	res->ParsePartialFromArray(d, sizeGoingToParse);
	inputBuf_.drain(sizeGoingToParse);
	return 0;
}

void StateConnection::dispatchMessage(const downloadmessage::Download_Response& res)
{
	using namespace downloadmessage;
	switch (res.state())
	{
	case Download_Response_State_DOWNLOADING:
		if (res.percent() < 1)
		{
			callback_->update(res.id(), res);
		}else
			callback_->finished(res.id());
		break;
	case Download_Response_State_PAUSED:
		callback_->paused(res.id());
		break;
	case Download_Response_State_REMOVED:
		callback_->removed(res.id());
		break;
	default:
		break;
	}
}
