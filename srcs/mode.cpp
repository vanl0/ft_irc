#include "Server.hpp"

void Server::mode(int fd, std::istringstream &msg){
	std::string	channelName;
	std::string	flags;
	std::string	params;
	bool		plusminus;

	msg >> channelName;
	if (channelName.empty() || channelName[0] != '#'){
		clients[fd].clientLog("Please provide a valid channel name and its mode\r\n", RED);	
		return;
	}
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	if (it == channels.end()){
		clients[fd].clientLog("Channel doesn't exist\r\n", RED);	
		return;
	}
	if (!it->second.isInChannel(&clients[fd])){
		clients[fd].clientLog("You don't belong to this channel, you can join using JOIN <channel> or getting invited\n", RED);
		return;
	}
	if (!it->second.isOperator(&clients[fd])){
		clients[fd].clientLog("This command can only be used by operators of the [" + channelName + "] channel\r\n", RED);
		return;
	}
	msg >> flags;
	if (flags.empty() || (flags[0] != '-' && flags[0] != '+') || flags.length() < 2){
		clients[fd].clientLog("Please provide a valid mode using -/+<mode> (parameter)\r\n", RED);
		return;
	}
	if (flags[0] == '-')
		plusminus = false;
	else
		plusminus = true;
	parseMode(fd, flags, msg, &it->second, params, plusminus);
	flags = params;
	if (!flags.empty() && (flags[0] == '-' || flags[0] == '+')){
		if (plusminus == false && flags[0] == '-')
			return (clients[fd].clientLog("Invalid repeated flag (-)\r\n", RED));
		if (plusminus == true && flags[0] == '+')
			return (clients[fd].clientLog("Invalid repeated flag (+)\r\n", RED));
		parseMode(fd, flags, msg, &it->second, params, !plusminus);
	}
	if (!params.empty())
		return (clients[fd].clientLog("Extra arguments ignored\r\n", RED));
}

void Server::parseMode(int fd, const std::string &flags, std::istringstream &msg, Channel *channel, std::string &params, bool plusminus){
	params = skipStream(msg);
	for (size_t i = 1; i < flags.length(); i++){
		if (flags[i] == 'i'){
			channel->inviteMode(&clients[fd], plusminus);
		}
		else if (flags[i] == 't'){
			channel->topicMode(&clients[fd], plusminus);
		}
		else if (flags[i] == 'k'){
			channel->keyMode(&clients[fd], plusminus, params);
			params = skipStream(msg);
		}
		else if (flags[i] == 'o'){
			channel->operatorMode(&clients[fd], plusminus, params);
			params = skipStream(msg);
		}
		else if (flags[i] == 'l'){
			channel->limitMode(&clients[fd], plusminus, params);
			params = skipStream(msg);
		} else {
			sendMsgFd(fd, std::string("Unrecognized mode: '") + flags[i] + "'\r\n", RED, WHI);
		}
	}
}

void Channel::inviteMode(Client *client, bool plusminus){
	setInviteFlag(plusminus);
	if (plusminus)
		sendtoMembers(client, "changed channel mode to invite only");
	else
		sendtoMembers(client, "changed channel mode to free access");
}

void Channel::topicMode(Client *client, bool plusminus){
	if (topicRights && plusminus)
		return(client->clientLog("Topic restrictions already set\r\n", RED));
	if (!topicRights && !plusminus)
		return(client->clientLog("Topic restrictions already unset\r\n", RED));
	setTopicRights(plusminus);
	if (plusminus){
		topicRights = true;
		sendtoMembers(client, "restricted topic rights to operators only");
	}else{
		topicRights = false;
		sendtoMembers(client, "cleared topic rights restrictions to all members");
	}
}

void Channel::keyMode(Client *client, bool plusminus, std::string &key){
	if (key.empty())
		return(client->clientLog("Please provide a valid key\r\n", RED));
	if (!keyFlag && !plusminus)
		return(client->clientLog("Cannot erase a password that's not set\r\n", RED));
	if (keyFlag && password != key)
		return(client->clientLog("Incorrect channel password, please provide a valid one\r\n", RED));
	if (plusminus){
		password = key;
		if (keyFlag)
			return(sendtoMembers(client, "changed the password of the channel"));
		else {
			keyFlag = true;
			return(sendtoMembers(client, "created a password for the channel"));
		}
	} else {
		keyFlag = false;
		password = std::string();
		return(sendtoMembers(client,"erased the password of the channel"));
	}
}

void Channel::operatorMode(Client *client, bool plusminus, std::string &user){
	Client	*userClient;
	if (user.empty())
		return(client->clientLog("Please provide a valid user\r\n", RED));
	if (!isInChannel(user))
		return(client->clientLog("User: " + user + " is not a member of [" + name + "] please provide a valid user\r\n", RED));
	userClient = getClient(user);
	if (isOperator(userClient)){
		if (plusminus)
			return(client->clientLog("User: " + user + " is already an operator of [" + name + "]\r\n", RED));
		removeOperator(userClient);
		sendtoMembers(client, "<" + user + "> is no longer an operator of [" + name + "]");
	} else {
		if (plusminus){
			addOperator(userClient);
			sendtoMembers(client, "<" + user + "> is now an operator of [" + name + "]");
		}else
			client->clientLog("User: " + user + " is not an operator of [" + name + "]\r\n", RED);
	}
}

void Channel::limitMode(Client *client, bool plusminus, std::string &limit){
	int	number;
	if (!plusminus){
		if (limitFlag){
			limitFlag = false;
			sendtoMembers(client, "erased the user limit on [" + name + "]");
		} else
			client->clientLog("This channel has already no user limit\r\n", RED);
	} else {
		if(limit.empty() || !isNumber(limit))
			return(client->clientLog("Please provide a valid user limit (between 1 and 100)\r\n", RED));
		number = std::atoi(limit.c_str());
		if (number <= 0 || number > USER_MAX)
			return(client->clientLog("Please provide a valid user limit (between 1 and 100)\r\n", RED));
		limitFlag = true;
		userLimit = number;
		sendtoMembers(client, "changed the user limit on [" + name + "] to " + limit);
	}
}
