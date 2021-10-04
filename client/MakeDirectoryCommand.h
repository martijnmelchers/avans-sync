#pragma once
#include <string>
#include <vector>

#include "ICommand.h"
#include "../AvanSync/Shared/Helpers.cpp"

class MakeDirectoryCommand : public ICommand
{
public:
	explicit MakeDirectoryCommand(asio::ip::tcp::iostream& client) : ICommand(client) {}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	void usage() override
	{
		writeClient("Incorrect usage. Usage: 'mkdir <directory> (parent directory)'");

	}
	int requiredArguments() override { return 1; }
};
