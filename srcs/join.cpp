#include "Server.hpp"

/*
https://datatracker.ietf.org/doc/html/rfc2812?utm_source=chatgpt.com#section-1.3
Channels names are strings (beginning with a '&', '#', '+' or '!')
of length up to 50 characters. It SHALL NOT contain
any spaces (' '), a control G (\a or ASCII 7), a comma (',').
*/
bool Server::isValidChannelName(const std::string &name)
{
	if (name.empty() || name.size() > 50)
		return (false);
	if (name.find_first_of("&#+!") != 0)
		return (false);
	if (name.find_first_of(" \a,") != std::string::npos)
		return (false);
	return (true);
}

void Server::join(int fd, std::istringstream& msg)
{
	std::string channelName;
	msg >> channelName;

	if (!isValidChannelName(channelName))
	{
		sendMsgFd(fd, "Bad syntax: Channel names start with '#', '&', '+' or '!' (max length 50)\n", RED);
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
	}
	else
		sendMsgFd(fd, "You are already in the [" + ch.getName() + "] channel.\n", MAG);
}
