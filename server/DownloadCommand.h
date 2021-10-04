#pragma once
#include "ICommand.h"
class DownloadCommand :
    public ICommand
{
public:
	explicit DownloadCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 2; }
};

