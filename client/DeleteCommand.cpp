#include "DeleteCommand.h"

#include <filesystem>



void DeleteCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	settings.state = READ;
	if(settings.syncSettings.state == NONE)
	{
		const std::filesystem::path path{ basePath + arguments[0] };

		if (!hasWritePermissions(status(path).permissions()))
		{
			writeClient("You don't have access to write to this directory.");
			settings.state = WAIT_INPUT;
			return;
		}

		if (!exists(path))
		{
			writeClient("This file or directory does exist on your client, checking with server.");
		}
		else
		{
			if (is_valid_path(path.string()))
			{
				remove_all(path);
				writeClient("File or directory have been deleted");
			}
			else
			{
				writeClient("Directory and file names cannot contain: \\ * ? < > : | \"");
				settings.state = WAIT_INPUT;
				return;
			}
		}
	}
	
	client_ << "del" << crlf;
	client_ << arguments[0] << crlf;
}
