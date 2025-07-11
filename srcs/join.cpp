#include "Server.hpp"

/*
https://datatracker.ietf.org/doc/html/rfc2812?utm_source=chatgpt.com#section-1.3
Channels names are strings (beginning with a '&', '#', '+' or '!') but only # is used
length up to 50 characters. It cannot contain any spaces (' '), a control G (\a), a comma (',').
*/
bool Server::isValidChannelName(const std::string &name)
{
	std::cout << name << std::endl;
	if (name.empty() || name.size() > 50)
		return (false);
	if (name[0] != '#')
		return (false);
	if (name.find_first_of(" \a,") != std::string::npos)
		return (false);
	return (true);
}

void Server::join(int fd, std::istringstream& msg)
{
	status = FAIL;
	std::string channelName;
	msg >> channelName;

	if (msg.peek() != EOF)
		return (sendMsgFd(fd, "Bad syntax: Too many arguments\n", RED));

	if (!isValidChannelName(channelName))
	{
		sendMsgFd(fd, "Bad syntax: Channel names start with '#' (max length 50)\n", RED);
		return ;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
		it = (channels.insert(std::make_pair(channelName, Channel(channelName)))).first;
	Channel& ch = it->second;
	if (!ch.isInChannel(fd))
	{
		ch.addUser(fd);
		sendMsgFd(fd, "You joined the [" + ch.getName() + "] channel. Welcome!\n", GRE);
		status = SUCCESS;
	}
	else
		sendMsgFd(fd, "You are already in the [" + ch.getName() + "] channel.\n", MAG);
}
