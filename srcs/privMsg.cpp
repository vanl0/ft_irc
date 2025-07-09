#include "Server.hpp"

void Server::privmsg(int fd, std::istringstream& msg)
{
	std::string dest;
	msg >> dest;
	if (dest.empty() || msg.peek() == EOF)
		return (sendMsgFd(fd, "Usage: PRIVMSG <#channel or user> :<message>\n", RED));

	std::string content;
	char c;
	msg >> c;
	if (c != ':')
		return (sendMsgFd(fd, "You forgot to put the ':' before the message\n", RED));

	std::getline(msg, content);
	if (content.empty())
		return (sendMsgFd(fd, "Your message is empty...\n", RED));
	if (dest[0] == '#')
	{
		// send message to channel
	}
	if (nickFd.find(dest) == nickFd.end())
		sendMsgFd(fd, "Can't send a message to a user that doesn't exist...\n", RED);
	else
	{
		sendMsgFd(nickFd[dest], clients[fd].getNick() + ": ", YEL);
		sendMsgFd(nickFd[dest], content + "\n", BLU);
		sendMsgFd(fd, "Message sent to user <" + dest + "> successfully\n", GRE);
	}
}