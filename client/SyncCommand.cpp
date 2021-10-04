#include "SyncCommand.h"
#include "../AvanSync/Shared/Helpers.cpp"


void SyncCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	const std::filesystem::path path{ basePath + arguments[0] };

	if(!is_directory(path) || !is_valid_path(path.string()))
	{
		writeClient("Path given isn't a directory, please pass a directory. To upload files use: upload <file>");
		settings.state = WAIT_INPUT;
		return;
	}
		
	settings.state = SYNC;
	settings.syncSettings.state = LIST_FILES;
	settings.catchOutput = true;

	settings.syncSettings.directoryQueue.push_back(path);
	discoverFiles(path, settings.syncSettings.fileQueue, settings.syncSettings.directoryQueue);
}

void SyncCommand::discoverFiles(const std::filesystem::path& path, std::deque<std::filesystem::path>& files, std::deque<std::filesystem::path>& directories) const
{
	for (const auto& p : std::filesystem::directory_iterator(path))
	{
		if (is_directory(p))
		{
			directories.push_back(p.path());
			discoverFiles(p.path(), files, directories);

		} else
		{
			files.push_back(p.path());
		}

	}
}
