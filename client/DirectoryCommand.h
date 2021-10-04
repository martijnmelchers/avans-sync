#pragma once
#include <string>
#include <vector>

#include "ICommand.h"
#include "../AvanSync/Shared/Helpers.cpp"

class DirectoryCommand : public ICommand
{
public:
	explicit DirectoryCommand(asio::ip::tcp::iostream& client) : ICommand(client) {}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	void usage() override
	{
		writeClient("Incorrect usage. Usage: 'dir <directory>'");
	}
	int requiredArguments() override { return 1; }

private:
	static bool hasEnding(const std::string& fullString, const std::string& ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}

		return false;
	}

};

