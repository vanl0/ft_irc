#include "Channel.hpp"

Channel::Channel(const std::string &channelName) : name(channelName),  topic("No topic is set."), topicRights(false) {};
		
std::string Channel::getName(void)
{
	return (this->name);
}

bool Channel::isInChannel(int fd) const
{
	return (this->members.find(fd) != this->members.end());
}

void Channel::addUser(int fd)
{
	if (!isInChannel(fd))
	{
		this->members.insert(fd);
		if (this->members.size() == 1)
			addOperator(fd);
	}
}

void Channel::removeUser(int fd)
{
	if (isInChannel(fd))
	{
		this->members.erase(fd);
		if (isOperator(fd))
			removeOperator(fd);
	}

}

void Channel::sendToAll(const std::string &msg)
{
	std::set<int>::iterator it;
	for (it = members.begin(); it != members.end(); ++it)
	{
		sendMsgFd(*it, "[" + getName() + "] ", MAG);
		sendMsgFd(*it, msg + "\n");
	}
}

/*
If we want to display what is the source of the message
example: user sends a message to the channel, topic change
*/
void Channel::sendToAll(const std::string &src, const std::string &msg)
{
	std::set<int>::iterator it;
	for (it = members.begin(); it != members.end(); ++it)
	{
		sendMsgFd(*it, "[" + getName() + "] ", MAG);
		sendMsgFd(*it,  src + ": ", BLU);
		sendMsgFd(*it, msg + "\n");
	}
}

std::string Channel::getTopic() const{
	return this->topic;
}
void Channel::setTopic(const std::string &newTopic){
	this->topic = newTopic;
	sendToAll("Topic has been set to", newTopic);
}

bool Channel::getTopicRights() const{
	return this->topicRights;
}

bool Channel::isOperator(int fd) const
{
	return (this->operators.find(fd) != this->operators.end());
}

void Channel::addOperator(int fd)
{
	if (!isOperator(fd))
		this->operators.insert(fd);
}

void Channel::removeOperator(int fd)
{
	if (isOperator(fd))
		this->operators.erase(fd);
}
