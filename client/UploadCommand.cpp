#include "UploadCommand.h"

void UploadCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	
	const std::filesystem::path path{ basePath + arguments[0] };

	if(!exists(path))
	{
		writeClient("File does not exist, please check the path");
		settings.state = WAIT_INPUT;
		return;
	}

	if(is_directory(path))
	{
		writeClient("You can only download files, directories are not supported.");
		settings.state = WAIT_INPUT;
		return;
	}
	
	settings.state = READ;
	settings.uploadSettings.path = path;
	
	client_ << "put" << crlf;
	client_ << arguments[0] << crlf;
	client_ << file_size(path) << crlf;
}

