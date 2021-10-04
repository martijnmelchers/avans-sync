#include "InfoCommand.h"

void InfoCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	settings.state = READ;
	client_ << "info" << crlf;
}
