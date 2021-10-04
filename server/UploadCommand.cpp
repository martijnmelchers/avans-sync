#include "UploadCommand.h"

#include "../AvanSync/Shared/Helpers.cpp"

void UploadCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	const std::filesystem::path path{ basePath + arguments[0] };

	if (!exists(path))
	{
		client_ << "ERROR: File does not exist" << crlf;
		return;
	}

	if (is_directory(path))
	{
		client_ << "ERROR: You can't download a directory" << crlf;
		return;
	}
	
	settings.state = UPLOAD;
	settings.uploadPath = path;

	client_ << file_size(path) << crlf;
}
