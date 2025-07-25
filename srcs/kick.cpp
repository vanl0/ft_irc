#include "Server.hpp"

void Server::kick(int fd, std::istringstream& msg) {
	status = FAIL;
	std::string chName;
    //std::string clientAddr;
    //msg >> clientAddr;
	msg >> chName;
	if (chName.empty() || msg.peek() == EOF)
		return (clients[fd].clientLog("Usage: KICK <channel> <name> (:<reason>)", RED));

    std::string name;
    msg >> name;
    if (name.empty())
		return (clients[fd].clientLog("Usage: KICK <channel> <name> (:<reason>)", RED));
	
    msg >> std::ws;
    std::string reason = "";
    std::getline(msg, reason);
    if (!reason.empty())
    {
	    if (reason[0] != ':')
        {
            clients[fd].clientLog("Usage: KICK <channel> <name> (:<reason>)\n", RED);
		    return (clients[fd].clientLog("You forgot to put the ':' before the reason", RED));
        }
        reason = reason.substr(1);
    }
	if (!isValidChannelName(chName))
		return (clients[fd].clientLog("Bad syntax: Channel names start with '&', '#', '+' or '!' (max length 50)", RED));
    if (channels.find(chName) == channels.end())
        return (clients[fd].clientLog("You can't kick someone from a nonexistent channel.", RED));
    Channel &ch = channels.at(chName);
    if (!ch.isInChannel(&clients[fd]))
        return (clients[fd].clientLog("You can't kick someone from a channel you are not part of.", RED));
    if (!ch.isOperator(&clients[fd]))
        return (clients[fd].clientLog("This command can only be used by operators of the [" + chName + "] channel", RED));
    if (nickFd.find(name) == nickFd.end())
        return (clients[fd].clientLog("You can't kick a nonexistent user.", RED));
    int fdKicked = nickFd[name];
    if (!ch.isInChannel(&clients[fdKicked]))
        return (clients[fd].clientLog(name + " is not part of the [" + chName + "] channel", RED));
    ch.removeUser(&clients[fdKicked]);
    if (!reason.empty())
        reason = " (reason: " + reason + ")";
    std::string srcNick = clients[fd].getNick(); 
    std::string channelMsg = name + " has been kicked out by " + srcNick + "." + reason;
    ch.sendtoMembers(channelMsg, RED);
    clients[fdKicked].clientLog("You have been kicked out of ", RED, true);
    clients[fdKicked].clientLog("[" + chName + "]", MAG, true);
    clients[fdKicked].clientLog(" by " + srcNick + "." + reason, RED);
    status = SUCCESS;
}