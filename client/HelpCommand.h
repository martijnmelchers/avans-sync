#pragma once
#include "ICommand.h"
#include "../AvanSync/Shared/Helpers.cpp"

class HelpCommand :
    public ICommand
{
public:
	explicit HelpCommand(asio::ip::tcp::iostream& client) : ICommand(client) {}

	void execute(std::vector<std::string>& arguments, Settings& settings) override
	{
		settings.state = WAIT_INPUT;
		usage();
	};
	
	void usage() override
	{
		writeClient("Available commands:");
		writeClient("info - Displays server info");
		writeClient("dir <directory> - Displays directory content");
		writeClient("mkdir <directory> (parent directory) - Make a new directory");
		writeClient("ren <old> <new> - Rename a file or directory");
		writeClient("upload <file> - Upload a file to the server");
		writeClient("download <file> - Download a file from the server");
		writeClient("sync <directory> - Sync your local directory with the server");
	}

};

