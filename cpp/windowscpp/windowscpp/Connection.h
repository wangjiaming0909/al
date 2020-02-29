#pragma once
#include <string>
#include <sockpp/inet_address.h>
#include <sockpp/connector.h>
#include <memory>
	
class Connection
{
public:
	Connection(const std::string& addr, uint32_t port);
	~Connection();

	bool connect();
	bool close();

	int64_t write(const void* data, size_t len);
	int64_t read(void* buf, size_t len);

	const sockpp::inet_address &getAddr() const;

private:
	in_addr_t parseAddr(const std::string& addr);

protected:
	std::shared_ptr<sockpp::inet_address> addrPtr_;
	std::shared_ptr<sockpp::connector> connector_;
};

