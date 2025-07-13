#include "Server.hpp"

void Server::mode(int fd, std::istringstream &msg){
	std::string	channelName;
	std::string	flags;
	std::string	params;
	bool		plusminus;

	msg >> channelName;
	if (channelName.empty() || channelName[0] != '#'){
		sendMsgFd(fd, "Please provide a valid channel name and its mode\r\n", RED, WHI);	
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()){
		sendMsgFd(fd, "Channel doesn't exist\r\n", RED, WHI);	
		return;
	}
	if (!it->second.isInChannel(fd)){
		sendMsgFd(fd, "You don't belong to this channel, you can join using JOIN <channel> or getting invited\n", RED, WHI);
		return;
	}
	if (!it->second.isOperator(fd)){
		sendMsgFd(fd, "This command can only be used by operators of the [" + channelName + "] channel\r\n", RED, WHI);
		return;
	}
	msg >> flags;
	if (flags.empty() || (flags[0] != '-' && flags[0] != '+') || flags.length() < 2){
		sendMsgFd(fd, "Please provide a valid mode using -/+<mode> (parameter)\r\n", RED, WHI);
		return;
	}
	if (flags[0] == '-')
		plusminus = false;
	else
		plusminus = true;
	parseMode(fd, flags, msg, &it->second, plusminus);
	if (!params.empty() && (params[0] == '-' || params[0] == '+')){
		if (plusminus == false && params[0] == '-')
			return (sendMsgFd(fd, "Invalid repeated flag (-)\r\n", RED, WHI));
		if (plusminus == true && params[0] == '+')
			return (sendMsgFd(fd, "Invalid repeated flag (+)\r\n", RED, WHI));
		parseMode(fd, params, msg, &it->second, !plusminus);
	}
}

void Server::parseMode(int fd, const std::string &flags, std::istringstream &msg, Channel *channel, bool plusminus){
	std::string params;
	msg >> params;

	for (size_t i = 1; i < flags.length(); i++){
		if (flags[i] == 'i'){
			channel->invite(fd, plusminus);
		}
		else if (flags[i] == 't'){
			channel->topicMode(fd, plusminus, params);
			msg >> params;
		}
		else if (flags[i] == 'k'){
			channel->keyMode(fd, plusminus, params);
			msg >> params;
		}
		else if (flags[i] == 'o'){
			channel->operatorMode(fd, plusminus, params);
			msg >> params;
		}
		else if (flags[i] == 'l'){
			channel->limitMode(fd, plusminus, params);
			msg >> params;
		} else {
			sendMsgFd(fd, std::string("Unrecognized mode: '") + flags[i] + "'\r\n", RED, WHI);
		}
	}
}

void Channel::invite(int fd, bool plusminus){
	(void)fd;
	(void)plusminus;
}

void Channel::topicMode(int fd, bool plusminus, std::string &topic){
	(void)fd;
	(void)plusminus;
	(void)topic;
}

void Channel::keyMode(int fd, bool plusminus, std::string &key){
	(void)fd;
	(void)plusminus;
	(void)key;
}

void Channel::operatorMode(int fd, bool plusminus, std::string &user){
	(void)fd;
	(void)plusminus;
	(void)user;
}

void Channel::limitMode(int fd, bool plusminus, std::string &limit){
	(void)fd;
	(void)plusminus;
	(void)limit;
}
