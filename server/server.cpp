//
// server.cpp
//
// Created by Bob Polis at 2020-12-02
//

#include <iostream>
#include <cstdlib>
#include <string>
#include <asio.hpp>
#include <fstream>



#include "CommandManager.h"
#include "DeleteCommand.h"
#include "DirectoryCommand.h"
#include "DownloadCommand.h"
#include "InfoCommand.h"
#include "RenameCommand.h"
#include "MakeDirectoryCommand.h"
#include "UploadCommand.h"


int main() {
	try {
		const int server_port{ 12345 };

		const char* lf{ "\n" };
		const char* crlf{ "\r\n" };

		asio::io_context io_context;
		asio::ip::tcp::acceptor server{ io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), server_port) };

		for (;;) {
			std::cerr << "waiting for client to connect\n";
			asio::ip::tcp::iostream client;
			server.accept(client.socket());
			std::cerr << "client connected from " << client.socket().local_endpoint() << lf;
			client << "Welcome to AvanSync server 1.0" << crlf;

			std::string command;
			std::vector<std::string> arguments;

			std::string input;

			bool receiveMode = false;

			// auto test = std::make_unique<InfoCommand>(InfoCommand(client));

			auto factory = new CommandManager(client);

			factory->registerCommand<InfoCommand>("info");
			factory->registerCommand<DirectoryCommand>("dir");
			factory->registerCommand<MakeDirectoryCommand>("mkdir");
			factory->registerCommand<RenameCommand>("ren");
			factory->registerCommand<DeleteCommand>("del");
			factory->registerCommand<UploadCommand>("get");
			factory->registerCommand<DownloadCommand>("put");

			Settings settings = { READ, { 0, "" } };

			while (client)
			{
				switch (settings.state)
				{
				case READ:
					if (getline(client, input)) {
						// Remove /r from argument
						input.erase(input.end() - 1);

						if (command.length() == 0)
							command = input;
						else
							arguments.push_back(input);


						factory->executeCommand(command, arguments, settings);
					}
					break;
				case UPLOAD:
				{
					std::ifstream fin(settings.uploadPath, std::ifstream::binary);
					std::vector<char> buffer(1024, 0);

					while (!fin.eof()) {
						fin.read(buffer.data(), buffer.size());
						std::streamsize s = fin.gcount();

						client.write(buffer.data(), s);
					}

					settings.uploadPath.clear();
					settings.state = READ;
				}

				break;
				case DOWNLOAD:
				{
					std::ofstream str;
					str.open(settings.downloadSettings.path, std::ofstream::binary);

					std::vector<char> buffer(settings.downloadSettings.totalBytes);


					client.read(buffer.data(), settings.downloadSettings.totalBytes);

					str.write(buffer.data(), buffer.size());
					str.close();

					client << "OK" << crlf;
					settings.state = READ;
				}
				break;
				}
			}
		}

	}
	catch (const std::exception& ex) {
		std::cerr << "server: " << ex.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
