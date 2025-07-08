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
	sendMsgFd(this->Fd, msg);
}

void Client::clientLog(const std::string &msg, const char *color) const {
	sendMsgFd(this->Fd, msg, color);
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

std::string Client::readMessage(int const fd){
	char	buffer[1024] = {0};
	ssize_t read_bytes = recv(fd, buffer, sizeof(buffer) - 1 , 0);
	
	if (read_bytes <= 0){
		return std::string();
	} else {
		return _partialMsg.append(buffer, read_bytes);
	}
}

std::string Client::appendPartial(char *buffer){
	if (!_partialMsg.empty())
		return _partialMsg.append(buffer);
	return buffer;
}

void Client::setPartial(const std::string &part){
	_partialMsg = part;
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
