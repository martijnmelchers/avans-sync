#pragma once
#include "ICommand.h"
class DeleteCommand :
    public ICommand
{
public:

	explicit DeleteCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}
	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 1; }
};

