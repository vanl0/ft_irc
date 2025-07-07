#include "Client.hpp"

Client::Client(){
	this->_isRegistered = true;
}

Client::~Client(){

}

std::string Client::getName() const{
	return this->_name;
}

std::string Client::getNick() const{
	return this->_name;
}

int Client::getFd() const{
	return this->_fd;
}


