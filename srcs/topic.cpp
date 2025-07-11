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
		sendMsgFd(fd, "Bad syntax: Channel names start with '#' (max length 50)\n", RED);
		return ;
	}
	if (channels.find(channelName) == channels.end()){
		client.clientLog("Channel [" + channelName + "] doesn't exist. To create one use JOIN <channel>\n", RED);
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (!it->second.isInChannel(fd)){
		client.clientLog("You don't belong to this channel, you can join using JOIN <channel> or getting invited\n", RED);
		return;
	}
	if (newTopic.empty()){
		client.clientLog("[" + channelName + "]", MAG);
		client.clientLog(" TOPIC: " + it->second.getTopic() + "\r\n");
		status = SUCCESS;
	} else {
		if (!it->second.isOperator(fd) && !it->second.getTopicRights())
			client.clientLog("You need to be an operator of [" + channelName + "] to change its topic\r\n", RED);
		else{
			it->second.setTopic(newTopic);
			client.clientLog("You have succesfully changed the topic of [" + channelName + "] to: ", GRE);
			client.clientLog(newTopic + "\n");
			status = SUCCESS;
		}
	}
}