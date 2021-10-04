#pragma once
#include "ICommand.h"
class UploadCommand :
    public ICommand
{
public:
	explicit UploadCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 1; }
};

