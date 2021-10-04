#include "DownloadCommand.h"
#include "../AvanSync/Shared/Helpers.cpp"

void DownloadCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	const std::filesystem::path path{ basePath + arguments[0] };

	if (!parent_directory_exists(path))
	{
		client_ << "ERROR: Parent directory doesn't exist." << crlf;
		return;
	}

	if (is_directory(path))
	{
		client_ << "ERROR: You can't download a directory" << crlf;
		return;
	}
	
	settings.state = DOWNLOAD;
	settings.downloadSettings.path = path;
	settings.downloadSettings.totalBytes = std::stoi(arguments[1]);

	client_ << "OK" << crlf;
}
