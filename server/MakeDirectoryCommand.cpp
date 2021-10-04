#include "MakeDirectoryCommand.h"

#include <filesystem>
#include "../AvanSync/Shared/Helpers.cpp"



void MakeDirectoryCommand::execute(std::vector<std::string>& arguments, Settings&)
{
	const std::filesystem::path path{ basePath + arguments[0] + "/" + arguments[1] };
	const std::filesystem::path parentPath{ basePath + arguments[0] };

	if (!hasWritePermissions(status(path).permissions()))
	{
		client_ << "ERROR: You don't have write permissions." << crlf;
		return;
	}

	if (is_directory(path))
	{
		client_ << "ERROR: Directory already exists." << crlf;
		return;
	}

	if (!is_directory(parentPath))
	{
		client_ << "ERROR: The parent directory " << parentPath << " does not exist, sub directories can't be created." << crlf;
		return;
	}

	try
	{
		if (is_valid_path(path.string()))
		{
			create_directories(path);
			client_ << "OK" << crlf;
		}
		else
		{
			client_ << "ERROR: Directory names cannot contain: \\ * ? < > : | \"" << crlf;
		}
	}
	catch (std::exception&)
	{
		client_ << "CLIENT >> Failed to create directory" << crlf;
	}
}

