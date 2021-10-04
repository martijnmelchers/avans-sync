#include "RenameCommand.h"

#include <filesystem>


void RenameCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	settings.state = READ;

	const std::filesystem::path path{ basePath + arguments[0] };
	const std::filesystem::path newPath{ basePath + arguments[1] };
	std::error_code code{};

	// We have to be able to write to the directory
	if (!hasReadAndWritePermissions(status(path).permissions()))
	{
		writeClient("You don't have access to write to this directory.");
		settings.state = WAIT_INPUT;
		return;
	}

	// If either path is invalid, we can't rename it.
	if (!is_valid_path(path.string()) || !is_valid_path(newPath.string()))
	{
		writeClient("Directory & file names cannot contain: \\ * ? < > : | \"");
		settings.state = WAIT_INPUT;
		return;
	}

	// If the new path already exist, we can't rename
	if (exists(newPath))
	{
		writeClient("A file with that name already exist, choose a different destination name.");
		settings.state = WAIT_INPUT;
		return;
	}

	try
	{
		if (exists(path))
		{
			rename(path, newPath);
			writeClient("File or directory has been renamed.");
		}
		else
		{
			writeClient("File or directory you want to rename does not exist.");
			settings.state = WAIT_INPUT;
			return;
		}

		client_ << "ren" << crlf;
		client_ << arguments[0] << crlf;
		client_ << arguments[1] << crlf;
	}
	catch (std::exception&)
	{
		writeClient("Failed to rename, check the file names");
		settings.state = WAIT_INPUT;
	}
}