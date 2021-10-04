#pragma once
#include "ICommand.h"
#include <filesystem>

class DirectoryCommand :
	public ICommand
{
public:
	explicit DirectoryCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 1; }

private:
	static std::string getType(const std::filesystem::directory_entry& entry);
};