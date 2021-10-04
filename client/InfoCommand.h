#pragma once
#include "ICommand.h"
class InfoCommand :
	public ICommand
{
public:
	explicit InfoCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	void usage() override {}
};