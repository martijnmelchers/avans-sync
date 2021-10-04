#pragma once
#include "ICommand.h"
class MakeDirectoryCommand :
	public ICommand
{
public:
	explicit MakeDirectoryCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 2; }
};
