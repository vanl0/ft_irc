#include "Server.hpp"

void Server::join(int fd, std::istringstream& msg)
{
	status = FAIL;
	std::string channelName;
	std::string password;
	msg >> channelName;
	msg >> password;
	//if (msg.peek() != EOF)
	//	return (clients[fd].clientLog("Bad syntax: Too many arguments\r\n", RED));

	if (!isValidChannelName(channelName))
	{
		clients[fd].clientLog("Bad syntax: Channel names start with '&', '#', '+' or '!' (max length 50)\r\n", RED);
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
			return (clients[fd].clientLog("You need to be invited to join [" + channelName + "]\r\n", RED));
		if (ch.isInvited(clients[fd].getNick()))
			ch.removeInvited(clients[fd].getNick());
//------PASSWORD
		if (ch.getKeyFlag()){
			if (password.empty())
				return (clients[fd].clientLog("Password is needed to join [" + channelName + "]\r\n", RED));
			if (!ch.isPassValid(password))
				return (clients[fd].clientLog("Channel password is incorrect\r\n", RED));
		}
//------USER LIMIT
		if (ch.getLimitFlag() && !ch.canUserJoin())
			return (clients[fd].clientLog("Channel [" + channelName + "] is currently full, try joining later\r\n", RED));
		ch.addUser(&clients[fd]);
		clients[fd].clientLog("You joined the [" + ch.getName() + "] channel. Welcome!\r\n", GRE);
		
		clients[fd].clientLog(":localhost 329 " + clients[fd].getNick() + " " + channelName + " " + getTimeString() + "\r\n");
		status = SUCCESS;
		/* clients[fd].clientLog(":" + clients[fd].getNick() + "!" + clients[fd].getUser(0) + "@localhost JOIN :" + channelName + "\r\n");
		clients[fd].clientLog(":localhost 353 " + clients[fd].getNick() + " = " + channelName + " :@" + clients[fd].getNick() + "\r\n");
		clients[fd].clientLog(":localhost 366 " + clients[fd].getNick() + " " + channelName + " :End of /NAMES list.\r\n"); */
	}
	else
		clients[fd].clientLog("You are already in the [" + ch.getName() + "] channel.\r\n", MAG);
}
