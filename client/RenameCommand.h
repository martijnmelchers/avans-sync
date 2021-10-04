#pragma once
#include "ICommand.h"
#include "../AvanSync/Shared/Helpers.cpp"


class RenameCommand :
	public ICommand
{
public:
	explicit RenameCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 2; }
	void usage() override
	{
		writeClient("Incorrect usage. Usage: 'ren <old> <new>'");
	}
};

