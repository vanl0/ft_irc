#include "Channel.hpp"

Channel::Channel(const std::string &channelName) : name(channelName) {};
		
std::string Channel::getName(void)
{
	return (this->name);
}

void Channel::addUser(int fd)
{
	if (isInChannel(fd))
		this->members.insert(fd);
	else
		sendMsgFd(fd, "User already in the [" + this->name + "] channel!", RED);
}

void Channel::removeUser(int fd)
{
	if (isInChannel(fd))
		this->members.erase(this->members.find(fd));
	else
		sendMsgFd(fd, "This user is not in the [" + this->name + "] channel!", RED);

}

bool Channel::isInChannel(int fd) const
{
	return (this->members.find(fd) != this->members.end());
}
