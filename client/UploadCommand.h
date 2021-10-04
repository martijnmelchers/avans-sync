#pragma once
#include "ICommand.h"
#include "../AvanSync/Shared/Helpers.cpp"

class UploadCommand :
    public ICommand
{
public:
	explicit UploadCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 1; }
	void usage() override
	{
		writeClient("Incorrect usage. Usage: 'upload <filename>'");
	};
};

