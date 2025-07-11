#include "Server.hpp"

void Server::privmsg(int fd, std::string& msg, std::string& chName)
{
	if (!isValidChannelName(chName) || channels.find(chName) == channels.end())
		return (clients[fd].clientLog("Can't send a message to a channel that doesn't exist...\n", RED));
	std::string srcNick = clients[fd].getNick();
	std::set<int> memberFds = channels.at(chName).getMembers();
	std::set<int>::iterator it;
	for (it = memberFds.begin(); it != memberFds.end(); ++it)
	{
		clients[*it].clientLog("[" + chName + "] ", MAG);
		clients[*it].clientLog(srcNick + ": ", BLU);
		clients[*it].clientLog( msg + "\n");
	}
	
}

void Server::privmsg(int fd, std::istringstream& msg)
{
	std::string dest;
	msg >> dest;
	if (dest.empty() || msg.peek() == EOF)
		return (clients[fd].clientLog("Usage: PRIVMSG <channel or user> :<message>\n", RED));

	std::string content;
	char c;
	msg >> c;
	if (c != ':')
		return (clients[fd].clientLog("You forgot to put the ':' before the message\n", RED));

	std::getline(msg, content);
	if (content.empty())
		return (clients[fd].clientLog("Your message is empty...\n", RED));
	if (dest[0] == '#')
		return (privmsg(fd, content, dest));
	if (nickFd.find(dest) == nickFd.end())
		clients[fd].clientLog("Can't send a message to a user that doesn't exist...\n", RED);
	else
	{
		clients[nickFd[dest]].clientLog(clients[fd].getNick() + ": ", BLU);
		clients[nickFd[dest]].clientLog(content + "\n");
		clients[fd].clientLog("Message sent to user <" + dest + "> successfully\n", GRE);
	}
}