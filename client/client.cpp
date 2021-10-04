//
// client.cpp
//
// Created by Bob Polis at 2020-12-02
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>
#include <asio.hpp>
#include <fstream>


#include "DirectoryCommand.h"
#include "HelpCommand.h"
#include "InfoCommand.h"
#include "MakeDirectoryCommand.h"
#include "DeleteCommand.h"
#include "CommandManager.h"
#include "RenameCommand.h"
#include "UploadCommand.h"
#include "DownloadCommand.h"
#include "SyncCommand.h"

#include "../AvanSync/Shared/Helpers.cpp"
#include "Settings.cpp"
#include "SyncManager.h"


int main() {
	try {
		const char* server_address{ "localhost" };
		const char* server_port{ "12345" };
		const char* prompt{ "avansync> " };
		const char* lf{ "\n" };
		const char* crlf{ "\r\n" };

		asio::ip::tcp::iostream server{ server_address, server_port };
		if (!server) throw std::runtime_error("could not connect to server");

		auto* cmdManager = new CommandManager(server);
		SyncManager syncManager{};

		cmdManager->registerCommand<HelpCommand>("help");
		cmdManager->registerCommand<DirectoryCommand>("dir");
		cmdManager->registerCommand<MakeDirectoryCommand>("mkdir");
		cmdManager->registerCommand<InfoCommand>("info");
		cmdManager->registerCommand<RenameCommand>("ren");
		cmdManager->registerCommand<DeleteCommand>("del");
		cmdManager->registerCommand<UploadCommand>("upload");
		cmdManager->registerCommand<DownloadCommand>("download");
		cmdManager->registerCommand<SyncCommand>("sync");


		// Command data
		std::string command;
		std::vector<std::string> arguments;

		std::string input;

		// Settings
		Settings settings{
			READ,
			0,
			false,
			{ 0, ""},
			{},
			{NONE}
		};
		while (server) {

			switch (settings.state)
			{
			case READ:
			{
				std::string resp;
				if (getline(server, resp)) {
					resp.erase(resp.end() - 1); // remove '\r'

					if (is_number(resp))
					{
						settings.pendingLines += std::stoi(resp);
					}
					else
					{
						if (!settings.catchOutput)
							writeServer(resp);
						else
							settings.syncSettings.serverOutput.push_back(resp);

						if (resp == "OK" && !settings.uploadSettings.path.empty())
						{
							settings.state = UPLOAD;
						}
						else if (resp != "OK" && !settings.uploadSettings.path.empty())
						{
							settings.uploadSettings.path.clear();
							settings.state = WAIT_INPUT;
						}
						else
						{
							if (settings.pendingLines == 0) {
								if (settings.catchOutput) {
									settings.state = SYNC;
								}
								else {
									settings.state = WAIT_INPUT;
								}
							}
							else
							{
								settings.pendingLines--;
							}
						}
					}
				}
			}

			break;

			case WAIT_INPUT:
			{
				std::cout << prompt;

				if (getline(std::cin, input)) {
					std::istringstream ss(input);

					while (ss >> input)
					{
						if (command.length() == 0)
							command = input;
						else
							arguments.push_back(input);
					}

					if (command == "quit") goto end;
					cmdManager->executeCommand(command, arguments, settings);
				}
			}
			break;

			case UPLOAD:
			{
				writeClient("Uploading file " + normalize_path(settings.uploadSettings.path));
				std::ifstream fin(settings.uploadSettings.path, std::ifstream::binary);
				std::vector<char> buffer(1024, 0);

				while (!fin.eof()) {
					fin.read(buffer.data(), buffer.size());
					std::streamsize s = fin.gcount();

					server.write(buffer.data(), s);
				}

				writeClient("Upload finished");

				settings.uploadSettings.path.clear();
				settings.state = READ;
			}

			break;

			case DOWNLOAD:
			{
				// Wait for server to send the bytes.
				std::string bytesStr;
				getline(server, bytesStr);
				bytesStr.erase(bytesStr.end() - 1); // remove '\r'


				// If the server didn't reply with a number, something went wrong.
				if (!is_number(bytesStr))
				{
					writeServer(bytesStr);
					settings.state = READ;
				}
				else
				{
					int bytes = std::stoi(bytesStr);

					writeClient("Download started");

					std::ofstream str;
					str.open(settings.downloadSettings.path, std::ofstream::binary);

					std::vector<char> buffer(bytes);
					server.read(buffer.data(), bytes);

					str.write(buffer.data(), buffer.size());
					str.close();

					writeClient("Download finished");
					settings.state = WAIT_INPUT;
				}
			}
			break;

			case SYNC:
				syncManager.execute(settings, cmdManager);
			break;
			}
		}
	end:;

	}
	catch (const std::exception& ex) {
		std::cerr << "client: " << ex.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
