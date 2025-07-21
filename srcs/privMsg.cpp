#include "Server.hpp"

void Server::privmsg(int fd, std::string& msg, std::string& chName)
{
	if (!isValidChannelName(chName) || channels.find(chName) == channels.end())
		return (clients[fd].clientLog("Can't send a message to a channel that doesn't exist...", RED));
	if (!channels.at(chName).isInChannel(&clients[fd]))
		return (clients[fd].clientLog("You don't belong to [" + chName + "]", RED));
	std::string srcNick = clients[fd].getNick();
	std::vector<Client *> members = channels.at(chName).getMembers();
	std::vector<Client *>::iterator it;
	for (it = members.begin(); it != members.end(); ++it)
	{
		(*it)->clientLog("[" + chName + "] ", MAG, true);
		(*it)->clientLog(srcNick + ": ", BLU, true);
		(*it)->clientLog(msg + "\r\n");
	}
}

void Server::privmsg(int fd, std::istringstream& msg)
{
	status = FAIL;
	std::string dest;
	msg >> dest;
	if (dest.empty() || msg.peek() == EOF)
		return (clients[fd].clientLog("Usage: PRIVMSG <channel or user> :<message>", RED));

	std::string content;
	char c;
	msg >> c;
	if (c != ':')
		return (clients[fd].clientLog("Usage: PRIVMSG <channel or user> :<message>\nYou forgot to put the ':' before the message", RED));

	std::getline(msg, content);
	if (content.empty())
		return (clients[fd].clientLog("Your message is empty...", RED));
	if (dest.find_first_of("&#+!") == 0)
		return (privmsg(fd, content, dest));
	if (nickFd.find(dest) == nickFd.end())
		clients[fd].clientLog("Can't send a message to a user that doesn't exist...", RED);
	else
	{
		clients[nickFd[dest]].clientLog(clients[fd].getNick() + ": ", BLU, true);
		clients[nickFd[dest]].clientLog(content + "\r\n");
		clients[fd].clientLog("Message sent to user <" + dest + "> successfully", GRE);
		status = SUCCESS;
	}
}