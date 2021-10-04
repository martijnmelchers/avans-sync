#pragma once
#include <string>
#include <asio/ip/tcp.hpp>
#include "Settings.cpp"

class ICommand
{
public:
	virtual ~ICommand() = default;
	explicit ICommand(asio::ip::tcp::iostream& client) : client_(client) {}
	virtual void execute(std::vector<std::string>& arguments, Settings& settings) = 0;
	virtual void usage() = 0;
	virtual int requiredArguments() { return 0; }

protected:
	asio::ip::tcp::iostream& client_;
	const char* lf{ "\n" };
	const char* crlf{ "\r\n" };
};
