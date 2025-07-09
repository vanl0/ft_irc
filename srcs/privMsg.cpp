#include "Server.hpp"

void Server::privmsg(int fd, std::string& msg, std::string& chName)
{
	if (!isValidChannelName(chName) || channels.find(chName) == channels.end())
		return (sendMsgFd(fd, "Can't send a message to a channel that doesn't exist...\n", RED));
	std::string srcNick = clients[fd].getNick();
	channels.at(chName).sendToAll(srcNick, msg);
}

void Server::privmsg(int fd, std::istringstream& msg)
{
	std::string dest;
	msg >> dest;
	if (dest.empty() || msg.peek() == EOF)
		return (sendMsgFd(fd, "Usage: PRIVMSG <channel or user> :<message>\n", RED));

	std::string content;
	char c;
	msg >> c;
	if (c != ':')
		return (sendMsgFd(fd, "You forgot to put the ':' before the message\n", RED));

	std::getline(msg, content);
	if (content.empty())
		return (sendMsgFd(fd, "Your message is empty...\n", RED));
	if (dest.find_first_of("&#+!") == 0)
		return (privmsg(fd, content, dest));
	if (nickFd.find(dest) == nickFd.end())
		sendMsgFd(fd, "Can't send a message to a user that doesn't exist...\n", RED);
	else
	{
		sendMsgFd(nickFd[dest], clients[fd].getNick() + ": ", BLU);
		sendMsgFd(nickFd[dest], content + "\n");
		sendMsgFd(fd, "Message sent to user <" + dest + "> successfully\n", GRE);
	}
}