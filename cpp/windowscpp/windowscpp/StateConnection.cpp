#include "StateConnection.h"

StateConnection::StateConnection(const std::string& addrStr, uint32_t port)
	: Connection(addrStr, port)
{
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

	}
}
