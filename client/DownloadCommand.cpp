#include "DownloadCommand.h"
#include "../AvanSync/Shared/Helpers.cpp"

void DownloadCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	const std::filesystem::path path{ basePath + arguments[0] };

	if(!parent_directory_exists(path))
	{
		writeClient("The file you're trying to download sits in a directory that doesn't exist. Create it with: 'mkdir <name>'");
		settings.state = WAIT_INPUT;
		return;
	}

	if (is_directory(path))
	{
		writeClient("You can only download files, directories are not supported.");
		settings.state = WAIT_INPUT;
		return;
	}
	
	settings.state = DOWNLOAD;
	settings.downloadSettings.path = path;

	client_ << "get" << crlf;
	client_ << arguments[0] << crlf;
}
