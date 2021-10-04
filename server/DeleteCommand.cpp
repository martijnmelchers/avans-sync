#include "DeleteCommand.h"

#include <filesystem>

#include "../AvanSync/Shared/Helpers.cpp"

void DeleteCommand::execute(std::vector<std::string>& arguments, Settings&)
{
	const std::filesystem::path path{ basePath + arguments[0] };

	if (!hasWritePermissions(status(path).permissions()))
	{
		client_ << "ERROR: You don't have access to write to this directory/file." << crlf;
		return;
	}

	try
	{
		if (!exists(path))
		{
			client_ << "ERROR: This file or directory does exist." << crlf;
		}
		else
		{
			if (is_valid_path(path.string()))
			{
				remove_all(path);
				client_ << "OK" << crlf;
			}
			else
			{
				client_ << "Directory and file names cannot contain: \\ * ? < > : | \"" << crlf;
			}
		}
	}
	catch (std::exception&)
	{
		client_ << "ERROR: Something went wrong while deleting" << crlf;;
	}
}
