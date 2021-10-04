
#include "InfoCommand.h"

void InfoCommand::execute(std::vector<std::string>& arguments, Settings& settings)
{
	client_ << "AvanSync Server 1.0 © Martijn Melchers" << crlf;
}