#include "Server.hpp"

void Server::privmsg(int fd, std::istringstream& msg)
{
	std::string restOfMsg;
	msg >> restOfMsg;

	std::cout << "[PRIVMSG] fd: " << fd << " msg:" << restOfMsg << std::endl;
}