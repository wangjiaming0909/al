#include "StateConnection.h"
#include <chrono>

StateConnection::StateConnection(const std::string& addrStr, uint32_t port)
	: Connection(addrStr, port), inputBuf_(), downloadRequests_{}
{
}

int StateConnection::download(int id, std::string& url)
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
	downloadRequests_.push(mess);
	return 0;
}

void StateConnection::setCallback(const std::shared_ptr<StateCallback> callback)
{
	callback_ = callback;
}

void StateConnection::startStateLoop()
{
	if (!connect())
	{
		std::cout << "connect failed..." << std::endl;
		return;
	}
	if (!setReadTimeout(std::chrono::milliseconds(1000)))
	{
		std::cout << "Set read timeout error..." << std::endl;
		return;
	}
	int respSize = sizeof(int) + sizeof(float) + sizeof(downloadmessage::Download_Response_State);
	while (this->connector_->is_open())
	{
		while (!downloadRequests_.empty())
		{
			auto mes = downloadRequests_.front();
			char arr[4096] = {};
			mes.SerializeToArray(arr, 4096);
			if (write(arr, mes.len()) != mes.len())
			{
				std::cout << "write error url: " << mes.url() << std::endl;
				downloadRequests_.pop();
				continue;
			}
			downloadRequests_.pop();
		}
		char d[4096] = {};
		auto sizeRead = read(d, 4096);
		if (sizeRead < 0 && inputBuf_.total_len() < respSize)
		{
			continue;
		}
		downloadmessage::Download_Response res;
		inputBuf_.append(d, sizeRead);
		while (inputBuf_.total_len() >= respSize)
		{
			decode(&res);
			dispatchMessage(res);
			if (res.state() == downloadmessage::Download_Response_State_FINISHED)
			{
				return;
			}
			res.Clear();
		}
	}
}

int StateConnection::decode(downloadmessage::Download_Response* res)
{
	using namespace downloadmessage;
	auto sizeGoingToParse = sizeof(int) + sizeof(float) + sizeof(downloadmessage::Download_Response_State);
	assert(inputBuf_.total_len() >= sizeGoingToParse);
	auto d = inputBuf_.pullup(sizeGoingToParse);
	res->ParsePartialFromArray(d, sizeGoingToParse);
	inputBuf_.drain(sizeGoingToParse);
	return 0;
}

void StateConnection::dispatchMessage(const downloadmessage::Download_Response& res)
{
	using namespace downloadmessage;
	if (!callback_)return;
	switch (res.state())
	{
	case Download_Response_State_DOWNLOADING:
			callback_->update(res.id(), res);
		break;
	case Download_Response_State_PAUSED:
		callback_->paused(res.id());
		break;
	case Download_Response_State_REMOVED:
		callback_->removed(res.id());
		break;
	case Download_Response_State_FAILED:
		callback_->failed(res.id());
		break;
	case Download_Response_State_FINISHED:
		callback_->finished(res.id());
		break;
	default:
		std::cout << "default" << std::endl;
		break;
	}
}

void SimpleStateCallback::update(int id, const downloadmessage::Download_Response& res)
{
	std::cout << "updating id: " << id << " percent: " << res.percent() * 100 << "%" << std::endl;
}

void SimpleStateCallback::paused(int id)
{
}

void SimpleStateCallback::resumed(int id)
{
}

void SimpleStateCallback::removed(int id)
{
}

void SimpleStateCallback::finished(int id)
{
	std::cout << "id: " << id << " findished..." << std::endl;
}

void SimpleStateCallback::failed(int id)
{
	std::cout << "id: " << id << " failed...." << std::endl;
}
