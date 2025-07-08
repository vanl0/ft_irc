#include "Channel.hpp"

Channel::Channel(const std::string &channelName) : name(channelName),  topic("No topic is set.\n"), topicRights(false) {};
		
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


std::string Channel::getTopic() const{
	return this->topic;
}
void Channel::setTopic(const std::string &newTopic){
	this->topic = newTopic;
	//here send message to every member showing the new topic
}

bool Channel::getTopicRights() const{
	return this->topicRights;
}
