#include "Server.hpp"

void Server::invite(int fd, std::istringstream &msg){
	std::string channelName;
	std::string nick;

	msg >> channelName;
	msg >> nick;
	if (channelName.empty() || channelName[0] != '#')
		return (clients[fd].clientLog("Please provide a valid channel name", RED));
	if (nick.empty())
		return (clients[fd].clientLog("Please provide a username to invite", RED));
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
		return (clients[fd].clientLog("Channel [" + channelName + "] doesn't exist", RED));
	if (!nickInUse(nick))
		return (clients[fd].clientLog("User <" + nick + "> doesn't exist", RED));
	if (it->second.isInChannel(nick))
		return (clients[fd].clientLog("User <" + nick + "> is already a member of [" + channelName + "]", RED));
	//if (it->second.isInvited(nick))
	//	return (clients[fd].clientLog("User <" + nick + "> is already invited to [" + channelName + "]"));
	it->second.addInvited(nick);
	std::map<int, Client>::const_iterator	mapIt = clients.begin();
	for (; mapIt != clients.end(); mapIt++){
		if (mapIt->second.getNick() == nick)
			mapIt->second.clientLog("You have been invited by <" + clients[fd].getNick() + "> to the [" + channelName + "] channel! To join use JOIN " + channelName, MAG);
	}
}