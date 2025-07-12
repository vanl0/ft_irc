#include "Server.hpp"

void Server::join(int fd, std::istringstream& msg)
{
	status = FAIL;
	std::string channelName;
	msg >> channelName;

	if (msg.peek() != EOF)
		return (clients[fd].clientLog("Bad syntax: Too many arguments\r\n", RED));

	if (!isValidChannelName(channelName))
	{
		clients[fd].clientLog("Bad syntax: Channel names start with '&', '#', '+' or '!' (max length 50)\r\n", RED);
		return ;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
		it = (channels.insert(std::make_pair(channelName, Channel(channelName)))).first;
	Channel& ch = it->second;
	if (!ch.isInChannel(fd))
	{
		ch.addUser(fd);
		clients[fd].clientLog("You joined the [" + ch.getName() + "] channel. Welcome!\r\n", GRE);
		status = SUCCESS;
	}
	else
		clients[fd].clientLog("You are already in the [" + ch.getName() + "] channel.\r\n", MAG);
}
