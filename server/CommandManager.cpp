#include "./CommandManager.h"

void CommandManager::executeCommand(std::string& command, std::vector<std::string>& arguments, Settings& settings)
{
	auto c = commands_.find(command);

	if (c != commands_.end())
	{
		// If we don't have the required amount of arguments, we need to wait for more input from the client.
		if (c->second->requiredArguments() != arguments.size())
			return;

		// Execute the command with the arguments.
		c->second->execute(arguments , settings);
	}
	else
	{
		client_ << "Command '" << command << "' is invalid." << crlf;
	}

	// Clear the command & arguments after it's been executed, or has failed.
	command.clear();
	arguments.clear();
}
