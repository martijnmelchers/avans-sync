#include "MakeDirectoryCommand.h"


#include <filesystem>
#include <iostream>



void MakeDirectoryCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	settings.state = READ;
	if (settings.syncSettings.state == NONE)
	{
		const std::filesystem::path path{ basePath + (arguments.size() == 1 ? "" : arguments[1] + "/") + arguments[0] };
		const std::filesystem::path parentPath{ basePath + (arguments.size() == 1 ? "" : arguments[1] + "/") };

		if (!hasWritePermissions(status(path).permissions()))
		{
			writeClient("You don't have access to write to this directory.");
			settings.state = WAIT_INPUT;
			return;
		}

		try
		{

			if (is_directory(path))
			{
				writeClient("Directory already exists. Checking with server");
			}
			else
			{
				// Can't make a folder in a non-existent parent folder
				if (!is_directory(parentPath))
				{
					writeClient("The parent directory " + arguments[1] + " does not exist, sub directories can't be created.");
					settings.state = WAIT_INPUT;
					return;
				}

				if (is_valid_path(path.string()))
				{
					create_directories(path);
					writeClient("Directory " + normalize_path(path) +" has been created.");
				}
				else
				{
					writeClient("Directory names cannot contain: \\ * ? < > : | \"");
					settings.state = WAIT_INPUT;
					return;
				}
			}
		}
		catch (std::exception&)
		{
			writeClient("Creating the directory failed.");
			settings.state = WAIT_INPUT;
		}

	}

	client_ << "mkdir" << crlf;
	client_ << (arguments.size() == 1 ? "" : arguments[1]) + "/" << crlf;
	client_ << arguments[0] << crlf;

}
