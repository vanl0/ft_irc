#include "Server.hpp"

void Server::topic(int fd, std::istringstream &msg){
	std::string channelName;
	std::string newTopic;
	Client		client = clients[fd];

	msg >> channelName;
	std::getline(msg >> std::ws, newTopic);

	if (channelName.empty()){
		client.clientLog("Bad syntax: Please provide a channel name to view/change its topic using TOPIC #<channel> :<new topic(optional)>\n");
		return;
	}
	if (channelName[0] != '#'){
		client.clientLog("Bad syntax: Channel names should start with #<channel>\n");
		return;
	}
	if (channels.find(channelName) == channels.end()){
		client.clientLog("Channel " + channelName + " doesn't exist. To create one use JOIN #<channel>\n", RED);
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (!it->second.isInChannel(fd)){
		client.clientLog("You don't belong to this channel, you can join using JOIN #<channel> or getting invited\n", RED);
		return;
	}
	if (newTopic.empty()){
		client.clientLog(channelName + " :");
		if (it->second.getTopic().empty())
			client.clientLog("No topic is set.\n");
		else
			client.clientLog(it->second.getTopic());
	} else {
		if (!it->second.isOperator(fd) && !it->second.getTopicRights())
			client.clientLog("You need to be an operator of " + channelName + "to change its topic\n", RED);
		else{
			it->second.setTopic(newTopic);
		}
	}
}