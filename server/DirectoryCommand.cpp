#include "DirectoryCommand.h"

#include <deque>
#include <filesystem>
#include "../AvanSync/Shared/Helpers.cpp"


void DirectoryCommand::execute(std::vector<std::string>& arguments, Settings&)
{
	const std::filesystem::path path{ basePath + arguments[0] };


	if(!hasReadPermissions(status(path).permissions()))
	{
		client_ << "ERROR: You don't have read permissions." << crlf;
		return;
	}
	
	if (is_directory(path) && is_valid_path(path.string()))
	{
		std::deque<std::string> queue;

		for (const auto& p : std::filesystem::directory_iterator(path))
		{
			auto ftime = last_write_time(p);
			std::time_t cftime = to_time_t<decltype(ftime)>(ftime);

			std::stringstream ss;
#pragma warning(suppress : 4996)
			ss << std::put_time(std::localtime(&cftime), "%F %T");

			queue.push_back(getType(p) + "|" + normalize_path(p.path()) + "|" + ss.str() +"|" + std::to_string(p.file_size()) + crlf);
		}

		if(queue.empty())
		{
			client_ << "Directory is empty." << crlf;

		} else
		{
			client_ << queue.size() - 1 << crlf;

			for (auto& i : queue)
				client_ << i;
		}

	}
	else
	{
		client_ << "ERROR: Directory " << arguments[0] << " doesn't exist!" << crlf;

	}
}

std::string DirectoryCommand::getType(const std::filesystem::directory_entry& entry)
{
	if (entry.is_directory())
		return "D";
	if (entry.is_other())
		return "*";

	return "F";
}
