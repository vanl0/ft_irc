#include "Server.hpp"

void Server::join(int fd, std::istringstream& msg)
{
	status = FAIL;
	std::string channelName;
	std::string password;
	msg >> channelName;
	msg >> password;
	//if (msg.peek() != EOF)
	//	return (clients[fd].clientLog("Bad syntax: Too many arguments", RED));

	if (!isValidChannelName(channelName))
	{
		clients[fd].clientLog("Bad syntax: Channel names start with '&', '#', '+' or '!' (max length 50)", RED);
		return ;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end())
		it = (channels.insert(std::make_pair(channelName, Channel(channelName)))).first;
	Channel& ch = it->second;
	if (!ch.isInChannel(&clients[fd]))
	{
//------INVITED
		if (ch.getInviteFlag() && !ch.isInvited(clients[fd].getNick()))
			return (clients[fd].clientLog("You need to be invited to join [" + channelName + "]", RED));
		
//------PASSWORD
		if (ch.getKeyFlag()){
			if (password.empty())
				return (clients[fd].clientLog("Password is needed to join [" + channelName + "]", RED));
			if (!ch.isPassValid(password))
				return (clients[fd].clientLog("Channel password is incorrect", RED));
		}
//------USER LIMIT
		if (ch.getLimitFlag() && !ch.canUserJoin())
			return (clients[fd].clientLog("Channel [" + channelName + "] is currently full, try joining later", RED));
		ch.addUser(&clients[fd]);
		clients[fd].clientLog("You joined the [" + ch.getName() + "] channel. Welcome!", GRE);
		if (ch.isInvited(clients[fd].getNick()))
			ch.removeInvited(clients[fd].getNick());
		// clients[fd].clientLog(":localhost 329 " + clients[fd].getNick() + " " + channelName + " " + getTimeString() + "");
		status = SUCCESS;
		/* clients[fd].clientLog(":" + clients[fd].getNick() + "!" + clients[fd].getUser(0) + "@localhost JOIN :" + channelName + "");
		clients[fd].clientLog(":localhost 353 " + clients[fd].getNick() + " = " + channelName + " :@" + clients[fd].getNick() + "");
		clients[fd].clientLog(":localhost 366 " + clients[fd].getNick() + " " + channelName + " :End of /NAMES list."); */
	}
	else
		clients[fd].clientLog("You are already in the [" + ch.getName() + "] channel.", MAG);
}
