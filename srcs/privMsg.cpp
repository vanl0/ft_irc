#include "Server.hpp"

void Server::privmsg(int fd, std::string& msg, std::string& chName)
{
	if (!isValidChannelName(chName) || channels.find(chName) == channels.end())
		return (clients[fd].clientLog("Can't send a message to a channel that doesn't exist...\r\n", RED));
	std::string srcNick = clients[fd].getNick();
	std::set<int> memberFds = channels.at(chName).getMembers();
	std::set<int>::iterator it;
	for (it = memberFds.begin(); it != memberFds.end(); ++it)
	{
		clients[*it].clientLog("[" + chName + "] ", MAG);
		clients[*it].clientLog(srcNick + ": ", BLU);
		clients[*it].clientLog( msg + "\r\n");
	}
	
}

void Server::privmsg(int fd, std::istringstream& msg)
{
	status = FAIL;
	std::string dest;
	msg >> dest;
	if (dest.empty() || msg.peek() == EOF)
		return (clients[fd].clientLog("Usage: PRIVMSG <channel or user> :<message>\r\n", RED));

	std::string content;
	char c;
	msg >> c;
	if (c != ':')
		return (clients[fd].clientLog("Usage: PRIVMSG <channel or user> :<message>\nYou forgot to put the ':' before the message\r\n", RED));

	std::getline(msg, content);
	if (content.empty())
		return (clients[fd].clientLog("Your message is empty...\r\n", RED));
	if (dest.find_first_of("&#+!") == 0)
		return (privmsg(fd, content, dest));
	if (nickFd.find(dest) == nickFd.end())
		clients[fd].clientLog("Can't send a message to a user that doesn't exist...\r\n", RED);
	else
	{
		clients[nickFd[dest]].clientLog(clients[fd].getNick() + ": ", BLU);
		clients[nickFd[dest]].clientLog(content + "\r\n");
		clients[fd].clientLog("Message sent to user <" + dest + "> successfully\r\n", GRE);
		status = SUCCESS;
	}
}