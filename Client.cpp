#include "Client.hpp"

int Client::getStatus() const{
	return this->loginStatus;
}

void Client::setNick(const std::string &nick){
	this->nick = nick;
}

std::string Client::getNick() const{
	return this->nick;
}

void Client::incrementStatus(){
	this->loginStatus++;
}

void Client::clientLog(const std::string &msg) const{
	send(this->Fd, msg.c_str(), msg.length(), 0);
}


void Client::setUser(const std::string &userName, const std::string &hostName, const std::string &serverName, const std::string &realName){
	this->user[0] = userName;
	this->user[1] = hostName;
	this->user[2] = serverName;
	this->user[3] = realName;
}

std::string Client::getUser(int i) const{
	if (i >= 0 && i <= 4)
		return this->user[i];
	else
		return NULL;
}

void Client::printLoginStatus() const{
	switch (loginStatus) {
	case 0:
		clientLog("Please provide a valid password using PASS <password>\n");
		break;
	case 1:
		clientLog("Please provide a nickname using NICK <nickname>\n");
		break;
	case 2:
		clientLog("Please provide a username using USER <username> <hostname(0)> <servername(*)> :<realname>\n");
		break;
	default:
		break;
	}	
}
