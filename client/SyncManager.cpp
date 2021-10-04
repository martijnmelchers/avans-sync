#include "SyncManager.h";
#include "../AvanSync/Shared/Helpers.cpp"

void SyncManager::execute(Settings& settings, CommandManager* manager)
{
	std::string command;
	std::vector<std::string> arguments;
	switch (settings.syncSettings.state)
	{


	case LIST_FILES:
	{
		for (const std::filesystem::path& path : settings.syncSettings.directoryQueue)
		{
			command = "dir";
			arguments.push_back(normalize_path(path.string()));
			manager->executeCommand(command, arguments, settings);
		}

		writeClient("[---] Starting sync with server...");
		writeClient("[1/5] Reading server directory content...");
		// Goto next state
		settings.syncSettings.state = READ_FILES;
		// Tell reader it can expect more than one line.
		settings.pendingLines += settings.syncSettings.directoryQueue.size() - 1;
	}
	break;


	case READ_FILES: {
		for (const auto& output : settings.syncSettings.serverOutput)
		{
			if (output.rfind("F|", 0) == 0 ||
				output.rfind("D|", 0) == 0 ||
				output.rfind("*|", 0) == 0)
			{
				settings.syncSettings.serverFiles.emplace_back(output);
			}
		}

		writeClient("[2/5] Validating server content...");

		// Clear output and go to next state
		settings.syncSettings.serverOutput.clear();
		settings.syncSettings.state = CREATE_DIRECTORIES;
	}
				   break;

	case CREATE_DIRECTORIES: {

		if (settings.syncSettings.directoryQueue.empty())
		{
			writeClient("[3/5] Created missing directories...");
			settings.syncSettings.state = UPLOAD_FILES;
		}
		else
		{
			std::vector<std::string> matchedDirectory;
			std::filesystem::path path = settings.syncSettings.directoryQueue.front();
			settings.syncSettings.directoryQueue.pop_front();

			for (auto it = settings.syncSettings.serverFiles.begin(); it != settings.syncSettings.serverFiles.end();)
			{
				std::stringstream ss(it->c_str());
				std::vector<std::string> parts;
				std::string part;

				while (std::getline(ss, part, '|'))
					parts.push_back(part);

				if (parts[1] == normalize_path(path))
				{
					settings.syncSettings.serverFiles.erase(it);
					matchedDirectory = parts;
					break;
				}

				++it;
			}

			if (matchedDirectory.empty())
			{
				writeClient("[3/5] Creating directory on server...");
				command = "mkdir";
				arguments.push_back(normalize_path(path.string()));
				manager->executeCommand(command, arguments, settings);
			}
		}

	}

	break;

	case UPLOAD_FILES: {

		if (settings.syncSettings.fileQueue.empty())
		{
			writeClient("[4/5] Uploaded missing/updated files...");
			settings.syncSettings.state = DELETE_FILES;
		}
		else
		{
			std::vector<std::string> matchedFile;
			std::filesystem::path path = settings.syncSettings.fileQueue.front();
			settings.syncSettings.fileQueue.pop_front();

			for (auto it = settings.syncSettings.serverFiles.begin(); it != settings.syncSettings.serverFiles.end();)
			{
				std::stringstream ss(it->c_str());
				std::vector<std::string> parts;
				std::string part;

				while (std::getline(ss, part, '|'))
					parts.push_back(part);

				if (parts[1] == normalize_path(path))
				{
					settings.syncSettings.serverFiles.erase(it);
					matchedFile = parts;
					break;
				}

				++it;
			}

			if (matchedFile.empty())
			{
				if (is_regular_file(path))
				{
					command = "upload";
					arguments.push_back(normalize_path(path.string()));
					manager->executeCommand(command, arguments, settings);
				}
			}
			else
			{
				auto ftime = last_write_time(path);
				std::time_t cftime = to_time_t<decltype(ftime)>(ftime);

				std::tm t = {};
				std::istringstream ss(matchedFile[2]);

				if (ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S"))
				{
					if (std::difftime(cftime, std::mktime(&t)) > 0)
					{
						if (is_regular_file(path))
						{
							command = "upload";
							arguments.push_back(normalize_path(path.string()));
							manager->executeCommand(command, arguments, settings);
						}
					}
				}
				else
				{
					writeClient("[4/5] Couldn't upload file " + normalize_path(path.string()));
				}
			}


		}
	}

	break;

	case DELETE_FILES: {


		if (settings.syncSettings.serverFiles.empty())
		{
			writeClient("[5/5] Deleted extra files on server...");
			writeClient("[---] Sync finished!");
			settings.syncSettings.state = NONE;
			settings.state = WAIT_INPUT;
			settings.catchOutput = false;

			// Clear any remaining data
			settings.syncSettings.serverFiles.clear();
			settings.syncSettings.serverOutput.clear();
			settings.syncSettings.directoryQueue.clear();
			settings.syncSettings.fileQueue.clear();

		}
		else
		{
			std::string data = settings.syncSettings.serverFiles.back();
			settings.syncSettings.serverFiles.pop_back();


			std::stringstream ss(data);
			std::vector<std::string> parts;
			std::string part;

			while (std::getline(ss, part, '|'))
				parts.push_back(part);

			writeClient("[5/5] Deleting file " + normalize_path(parts[1]) + "...");

			// Delete the file if it no longer exists on remote
			command = "del";
			arguments.push_back(normalize_path(parts[1]));
			manager->executeCommand(command, arguments, settings);
		}

	}
	}
}
