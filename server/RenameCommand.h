#pragma once
#include "ICommand.h"
class RenameCommand :
	public ICommand
{
public:
	explicit RenameCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 2; }
};
