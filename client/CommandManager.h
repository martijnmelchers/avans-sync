#pragma once
#include <map>
#include <string>
#include "ICommand.h"

class CommandManager
{
public:
	explicit CommandManager(asio::ip::tcp::iostream& client) : client_(client) {}

	template<typename T>
	void registerCommand(std::string command)
	{
		static_assert(std::is_base_of<ICommand, T>::value,
			"T must inherit from ICommand");

		commands_.insert(std::make_pair(command, std::make_unique<T>(client_)));
	}
	void executeCommand(std::string& command, std::vector<std::string>& arguments, Settings& settings);

private:
	std::map<std::string, std::unique_ptr<ICommand>> commands_;
	asio::ip::tcp::iostream& client_;

	const char* lf{ "\n" };
	const char* crlf{ "\r\n" };

};

