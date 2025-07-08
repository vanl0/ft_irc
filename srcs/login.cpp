#include "Server.hpp"

void Server::pass(int fd, std::istringstream& msg) {
	std::string	clientPass;

	if (clients[fd].getStatus() > 0){
		clients[fd].clientLog("You've already put the correct password\n");
		clients[fd].printLoginStatus();
		return;
	}
	msg >> clientPass;
	if (!isEmpty(msg)){
		clients[fd].clientLog("Please provide only one password\n");
		return;
	}
	if (clientPass == this->password){
		clients[fd].clientLog("Password correct! ", GRE);
		clients[fd].incrementStatus();
		clients[fd].printLoginStatus();
	} else {
		clients[fd].clientLog("Password incorrect, provide a password using PASS <password>\n");
	}
}

bool Server::validNick(const std::string &nick){
	std::map<int, Client>::const_iterator	it = clients.begin();
	for(; it != clients.end(); it++){
		if(it->second.getNick() == nick)
			return false;
	}
	return true;
}

void Server::nick(int fd, std::istringstream& msg) {
	std::string clientNick;

	if (clients[fd].getStatus() < 1){
		clients[fd].clientLog("Missing password: ");
		clients[fd].printLoginStatus();
		return;
	}
	msg >> clientNick;
	if (!isEmpty(msg) || clientNick.empty()){
		clients[fd].clientLog("Wrong syntax: ");
		clients[fd].printLoginStatus();
		return;
	}
	if (!validNick(clientNick)){
		clients[fd].clientLog("This nick is already in use\n", RED);
		clients[fd].printLoginStatus();
		return;
	} else {
		clients[fd].setNick(clientNick);
		nickFd[clientNick] = fd; // added to quickly convert string (nick) to int (fd)
		clients[fd].clientLog("Nickname set successfuly!\n", GRE);
		if (clients[fd].getStatus() == 1){
			clients[fd].incrementStatus();
			clients[fd].printLoginStatus();
		}
	}
}

void Server::user(int fd, std::istringstream& msg) {
	std::string clientUser;
	std::string clientHostname;
	std::string clientServername;
	std::string	clientRealname;

	if (clients[fd].getStatus() == 0){
		clients[fd].clientLog("Missing password: ");
		clients[fd].printLoginStatus();
		return;
	}
	if (clients[fd].getStatus() == 1){
		clients[fd].clientLog("Missing nick: ");
		clients[fd].printLoginStatus();
		return;
	}
	if (clients[fd].getStatus() > 2){
		clients[fd].clientLog("Your user has already been set up! It cannot be changed\n");
		return;
	}
	msg >> clientUser >> clientHostname >> clientServername;
	std::getline(msg >> std::ws, clientRealname);
	if (clientUser.empty() || clientHostname.empty() ||\
		clientServername.empty() || clientRealname.empty() ||\
		clientRealname[0] != ':')
	{
			clients[fd].clientLog("Wrong syntax: ");
			clients[fd].printLoginStatus();
			return;
	}
	clientRealname.erase(0, 1);
	clients[fd].setUser(clientUser, clientHostname, clientServername, clientHostname);
	clients[fd].incrementStatus();
	clients[fd].clientLog("User set up succesfully, welcome!\n", GRE);
}
