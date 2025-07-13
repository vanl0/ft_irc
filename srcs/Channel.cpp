#include "Channel.hpp"

Channel::Channel(const std::string &channelName)
	: name(channelName), topic("No topic is set."), inviteFlag(false),
	topicRights(false), keyFlag(false), operatorFlag(false), limitFlag(false) {};
		
std::string Channel::getName(void)
{
	return (this->name);
}

std::set<int> Channel::getMembers() const
{
	return (this->members);
}

std::string Channel::getTopic() const{
	return (this->topic);
}

int	Channel::getLimit(void) const
{
	return (this->limit);
}

bool Channel::getInviteFlag(void) const
{
	return (this->inviteFlag);
}

bool Channel::getTopicRights() const
{
	return (this->topicRights);
}

bool Channel::getKeyFlag(void) const
{
	return (this->keyFlag);
}

bool Channel::getOperatorFlag(void) const
{
	return (this->operatorFlag);
}

bool Channel::getLimitFlag(void) const
{
	return (this->limitFlag);
}

void Channel::setInviteFlag(bool flag)
{
	this->inviteFlag = flag;
}

void Channel::setTopicRights(bool flag)
{
	this->topicRights = flag;
}

void Channel::setKeyFlag(bool flag)
{
	this->keyFlag = flag;
}

void Channel::setOperatorFlag(bool flag)
{
	this->operatorFlag = flag;
}
void Channel::setLimitFlag(bool flag)
{
	this->limitFlag = flag;
}

void Channel::setTopic(const std::string &newTopic)
{
	this->topic = newTopic;
}

void Channel::setPassword(std::string const &pass)
{
	this->password = pass;
}

void Channel::setLimit(int newLimit)
{
	this->limit = newLimit;
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
