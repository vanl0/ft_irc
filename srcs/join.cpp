#include "Server.hpp"

void Server::join(int fd, std::istringstream& msg)
{
    std::string channelName;
    msg >> channelName;

    if (channelName.empty() || channelName[0] != '#')
	{
        sendMsgFd(fd, "Invalid channel name\n", RED);
        return;
    }
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end())
	{
    	it = channels.insert(std::make_pair(channelName, Channel(channelName))).first;
	}
	Channel& ch = it->second;
	if (!ch.isInChannel(fd))
	{
		ch.addUser(fd);
		sendMsgFd(fd, "You joined the [" + ch.getName() + "] channel. Welcome!\n", GRE);
	}
	else
		sendMsgFd(fd, "You are already in the [" + ch.getName() + "] channel!\n", RED);
}