#include "Server.hpp"

void Server::mode(int fd, std::istringstream &msg){
	std::string	channelName;

	msg >> channelName;
	if (channelName.empty() || channelName[0] != '#'){
		sendMsgFd(fd, "Please provide a valid channel name and its mode\r\n", RED);	
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()){
		sendMsgFd(fd, "Channel doesn't exist\r\n", RED);	
		return;
	}
	if (!it->second.isInChannel(fd)){
		sendMsgFd(fd, "You don't belong to this channel, you can join using JOIN <channel> or getting invited\n", RED);
		return;
	}
	if (!it->second.isOperator(fd)){
		
	}
}
