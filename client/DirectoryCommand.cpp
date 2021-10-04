#include "DirectoryCommand.h"


void DirectoryCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	settings.state = READ;
	client_ << "dir" << crlf;
	client_ << (hasEnding(arguments[0], "/") ? arguments[0] : arguments[0] + "/") << crlf;
}
