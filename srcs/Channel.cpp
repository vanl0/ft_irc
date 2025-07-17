#include "Channel.hpp"

Channel::Channel(const std::string &channelName)
	: name(channelName), topic("No topic is set."), inviteFlag(false),
	topicRights(false), keyFlag(false), operatorFlag(false), limitFlag(false) {};

void Channel::sendtoMembers(const std::string &msg, const char *color){
	std::vector<Client *>::iterator	it;
	for (it = members.begin(); it != members.end(); it++){
		(*it)->clientLog(msg, color);
	}
}

void Channel::sendtoMembers(Client *client, const std::string &msg){
	std::vector<Client *>::iterator	it;
	for (it = members.begin(); it != members.end(); it++){
		(*it)->clientLog("[" + name + "] ", MAG);
		(*it)->clientLog(client->getNick() + ": ", BLU);
		(*it)->clientLog(msg + "\r\n", MAG);
	}
}

std::string Channel::getName(void) const{
	return (this->name);
}

std::vector<Client *> Channel::getMembers() const{
	return (this->members);
}

std::string Channel::getTopic() const{
	return (this->topic);
}

int	Channel::getLimit(void) const
{
	return (this->userLimit);
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
	this->userLimit = newLimit;
}

bool Channel::isInChannel(Client *client) const
{
	std::vector<Client *>::const_iterator it = std::find(members.begin(), members.end(), client);
	return (it != members.end());
}

bool Channel::isInChannel(const std::string &nick) const{
	std::vector<Client *>::const_iterator	it;
	for (it = members.begin(); it != members.end(); it++){
		if ((*it)->getNick() == nick)
			return (true);
	}
	return false;
}

Client *Channel::getClient(const std::string &nick) const{
	std::vector<Client *>::const_iterator	it;
	for (it = members.begin(); it != members.end(); it++){
		if ((*it)->getNick() == nick)
			return (*it);
	}
	return NULL;
}
void Channel::addUser(Client *client)
{
	if (!isInChannel(client))
	{
		this->members.push_back(client);
		if (this->members.size() == 1)
			addOperator(client);
	}
}

void Channel::removeUser(Client *client)
{
	if (isInChannel(client))
	{	
		members.erase(std::remove(members.begin(), members.end(), client), members.end());
		if (isOperator(client))
			removeOperator(client);
	}
}

bool Channel::isOperator(Client *client) const
{
	std::vector<Client *>::const_iterator it = std::find(operators.begin(), operators.end(), client);
	return (it != operators.end());
}

void Channel::addOperator(Client *client)
{
	if (!isOperator(client))
		this->operators.push_back(client);
}

void Channel::removeOperator(Client *client)
{
	if (isOperator(client))
		operators.erase(std::remove(operators.begin(), operators.end(), client), operators.end());
}

bool Channel::isInvited(const std::string &nick) const{
	std::vector<std::string>::const_iterator it;
	for (it = invitedUsers.begin(); it != invitedUsers.end(); it++){
		if (nick == *it)
			return true;
	}
	return false;
}

void Channel::addInvited(const std::string &nick){
	invitedUsers.push_back(nick);
}

void Channel::removeInvited(const std::string &nick){
	if (isInvited(nick))
		invitedUsers.erase(std::remove(invitedUsers.begin(), invitedUsers.end(), nick), invitedUsers.end());
}

bool Channel::isPassValid(const std::string &pass) const{
	return (pass == password);
}

int Channel::canUserJoin() const{
	return ((limitFlag && static_cast<int>(members.size()) < userLimit) || !limitFlag);
}