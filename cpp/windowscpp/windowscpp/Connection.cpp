#include "Connection.h"
#include <sockpp/exception.h>

Connection::Connection(const std::string& addrStr, uint32_t port)
{
    auto addr = parseAddr(addrStr);
    addrPtr_ = std::make_shared<sockpp::inet_address>(addr, port);
}

Connection::~Connection()
{
}

bool Connection::connect()
{
    return connector_->connect(*addrPtr_);
}

bool Connection::close()
{
    return connector_->close();
}

int64_t Connection::write(const void* data, size_t len)
{
    return connector_->write(data, len);
}

int64_t Connection::read(void* buf, size_t len)
{
    return connector_->read(buf, len);
}

const sockpp::inet_address& Connection::getAddr() const
{
    return *addrPtr_;
}

in_addr_t Connection::parseAddr(const std::string& addrStr)
{
    in_addr_t addr{};
    try
    {
        addr = sockpp::inet_address::resolve_name(addrStr);
    }catch(const sockpp::getaddrinfo_error& ex)
    {
        std::cout << ex.what() << std::endl;
    }catch(const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return addr;
}
