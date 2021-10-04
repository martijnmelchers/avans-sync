#include "RenameCommand.h"


#include <filesystem>

#include "../AvanSync/Shared/Helpers.cpp"


void RenameCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	const std::filesystem::path path{ basePath + arguments[0] };
	const std::filesystem::path newPath{ basePath + arguments[1] };

	// We have to be able to write to the directory
	if (!hasReadAndWritePermissions(status(path).permissions()))
	{
		client_ << "ERROR: You don't have access to write to this directory." << crlf;
		return;
	}

	// If either path is invalid, we can't rename it.
	if (!is_valid_path(path.string()) || !is_valid_path(newPath.string()))
	{
		client_ << "ERROR: Directory & file names cannot contain: \\ * ? < > : | \"" << crlf;
		return;
	}

	// If the new path already exist, we can't rename
	if (exists(newPath))
	{
		client_ << "ERROR: A file with that name already exist, choose a different destination name." << crlf;
		return;
	}


	try
	{
		if (exists(path))
		{
			rename(path, newPath);
			client_ << "OK" << crlf;
		}
		else
		{
			client_ << "ERROR: File or directory does not exist" << crlf;
		}
	}
	catch (std::exception&)
	{
		client_ << "ERROR: Failed to rename, check the file names" << crlf;
	}
}
