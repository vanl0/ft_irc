#include "Server.hpp"

void Server::topic(int fd, std::istringstream &msg){
	std::string channelName;
	std::string newTopic;
	Client		client = clients[fd];
	status = FAIL;

	msg >> channelName;
	std::getline(msg >> std::ws, newTopic);

	if (!isValidChannelName(channelName))
	{
		client.clientLog("Bad syntax: Channel names start with '&', '#', '+' or '!' (max length 50)", RED);
		return ;
	}
	if (channels.find(channelName) == channels.end()){
		client.clientLog("Channel [" + channelName + "] doesn't exist. To create one use JOIN <channel>", RED);
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (!it->second.isInChannel(&clients[fd])){
		client.clientLog("You don't belong to this channel, you can join using JOIN <channel> or getting invited", RED);
		return;
	}
	if (newTopic.empty()){
		client.clientLog("[" + channelName + "]", MAG, true);
		client.clientLog(" TOPIC: " + it->second.getTopic() + "\r\n");
		status = SUCCESS;
	} else {
		if (!it->second.isOperator(&clients[fd]) && it->second.getTopicRights())
			client.clientLog("You need to be an operator of [" + channelName + "] to change its topic", RED);
		else if (newTopic[0] != ':')
			client.clientLog("You forgot to put the ':' before the new topic", RED);
		else{
			newTopic = newTopic.substr(1);
			it->second.setTopic(newTopic);
			std::string topicMsg = "Topic set to: " + newTopic;
			privmsg(fd, topicMsg, channelName);
			client.clientLog("You have succesfully changed the topic of [" + channelName + "] to: ", GRE, true);
			client.clientLog(newTopic + "\r\n");
			status = SUCCESS;
		}
	}
}