#include "./CommandManager.h"

#include <iostream>


void CommandManager::executeCommand(std::string& command, std::vector<std::string>& arguments, Settings& settings)
{
	auto c = commands_.find(command);

	if (c != commands_.end())
	{
		if (c->second->requiredArguments() > arguments.size())
		{
			// If we don't have the required amount of arguments, we show the usage
			c->second->usage();
			settings.state = WAIT_INPUT;
		}
		else
		{
			// Execute the command with the arguments.
			c->second->execute(arguments, settings);
		}
	}
	else
	{
		std::cout << "Command '" << command << "' is invalid." << crlf;
		settings.state = WAIT_INPUT;
	}

	// Clear the command & arguments after it's been executed, or has failed.
	command.clear();
	arguments.clear();
}
