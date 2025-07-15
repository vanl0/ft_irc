#include "Server.hpp"

void Server::kick(int fd, std::istringstream& msg) {
	status = FAIL;
	std::string chName;
	msg >> chName;
	if (chName.empty() || msg.peek() == EOF)
		return (clients[fd].clientLog("Usage: KICK <channel> <name> (:<reason>)\r\n", RED));

    std::string name;
    msg >> name;
    if (name.empty())
		return (clients[fd].clientLog("Usage: KICK <channel> <name> (:<reason>)\r\n", RED));
	
    msg >> std::ws;
    std::string reason = "";
    std::getline(msg, reason);
    if (!reason.empty())
    {
	    if (reason[0] != ':')
        {
            clients[fd].clientLog("Usage: KICK <channel> <name> (:<reason>)\n", RED);
		    return (clients[fd].clientLog("You forgot to put the ':' before the reason\r\n", RED));
        }
        reason = reason.substr(1);
    }
	if (!isValidChannelName(chName))
		return (clients[fd].clientLog("Bad syntax: Channel names start with '&', '#', '+' or '!' (max length 50)\r\n", RED));
    if (channels.find(chName) == channels.end())
        return (clients[fd].clientLog("You can't kick someone from a nonexistent channel.\r\n", RED));
    Channel &ch = channels.at(chName);
    if (!ch.isInChannel(&clients[fd]))
        return (clients[fd].clientLog("You can't kick someone from a channel you are not part of.\r\n", RED));
    if (!ch.isOperator(&clients[fd]))
        return (clients[fd].clientLog("This command can only be used by operators of the [" + chName + "] channel\r\n", RED));
    if (nickFd.find(name) == nickFd.end())
        return (clients[fd].clientLog("You can't kick a nonexistent user.\r\n", RED));
    int fdKicked = nickFd[name];
    if (!ch.isInChannel(&clients[fd]))
        return (clients[fd].clientLog(name + " is not part of the [" + chName + "] channel\r\n", RED));
    ch.removeUser(&clients[fd]);
    if (!reason.empty())
        reason = " (reason: " + reason + ")";
    std::string srcNick = clients[fd].getNick(); 
    std::string channelMsg = name + " has been kicked out by " + srcNick + "." + reason;
    privmsg(fd, channelMsg, chName);
    clients[fdKicked].clientLog("You have been kicked out of ", RED);
    clients[fdKicked].clientLog("[" + chName + "]", MAG);
    clients[fdKicked].clientLog(" by " + srcNick + "." + reason + "\r\n", RED);
    status = SUCCESS;
}