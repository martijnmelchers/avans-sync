#pragma once
#include "ICommand.h"
#include "../AvanSync/Shared/Helpers.cpp"

class SyncCommand :
    public ICommand
{
public:
	explicit SyncCommand(asio::ip::tcp::iostream& client) : ICommand(client) {
	}

	void execute(std::vector<std::string>& arguments, Settings& settings) override;
	int requiredArguments() override { return 1; }
	void usage() override
	{
		writeClient("Incorrect usage. Usage: 'sync <directory>'");
	}

private:
	void discoverFiles(const std::filesystem::path& path, std::deque<std::filesystem::path>& files, std::deque<std::filesystem::path>& directories) const;
};

