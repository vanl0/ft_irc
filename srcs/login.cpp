#include "Server.hpp"


void Server::pass(int fd, std::istringstream& msg) {
	std::string	clientPass;
	status = FAIL;
	if (clients[fd].getStatus() > 0){
		clients[fd].clientLog("You've already put the correct password.", RED);
		clients[fd].printLoginStatus();
		return ;
	}
	msg >> clientPass;
	if (!isEmpty(msg)){
		clients[fd].clientLog("Please provide only one password using PASS <password>", RED);
		return ;
	}
	if (clientPass == this->password){
		clients[fd].clientLog("Password correct!", GRE);
		clients[fd].clientLog("Welcome to ircserv.", BLU);
		clients[fd].incrementStatus();
		clients[fd].printLoginStatus();
		status = SUCCESS;
	} else {
		clients[fd].clientLog("Password incorrect!", RED);
		clients[fd].printLoginStatus();
	}
}

/*
RFC 1459: https://datatracker.ietf.org/doc/html/rfc1459#section-2.3.1
<nick>= <letter> { <letter> | <number> | <special> }
<special> = '-' | '[' | ']' | '\' | '`' | '^' | '{' | '}'
*/
bool isValidNick(const std::string& nick) {
    if (nick.empty() || nick.size() > 9 || !std::isalpha(nick[0]))
        return (false);
    for (size_t i = 1; i < nick.size(); ++i) {
        char c = nick[i];
        if (!isalnum(c) && c != '-' && c != '['
			&& c != ']' && c != '\\' && c != '`'
			&& c != '^' && c != '{' && c != '}')
            return (false);
    }
    return (true);
}

bool Server::nickInUse(const std::string &nick)
{
	std::map<int, Client>::const_iterator	it;
	for (it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second.getNick() == nick)
			return (true);
	}
	return (false);
}

void Server::nick(int fd, std::istringstream& msg) {
	std::string clientNick;
	status = FAIL;

	if (clients[fd].getStatus() < 1){
		clients[fd].clientLog("Missing password.", RED);
		clients[fd].printLoginStatus();
		return ;
	}
	msg >> clientNick;
	if (!isEmpty(msg) || !isValidNick(clientNick)){
		clients[fd].clientLog("Wrong nickname syntax. Must start with a letter (max length 9).", RED);
		clients[fd].printLoginStatus();
		return ;
	}
	if (nickInUse(clientNick)){
		clients[fd].clientLog("This nickname is already in use.", RED);
		clients[fd].printLoginStatus();
		return ;
	} else {
		if (clients[fd].getStatus() > 1) // if nick has been set, we remove it
			nickFd.erase(clients[fd].getNick());
		clients[fd].setNick(clientNick);
		nickFd[clientNick] = fd; // added to quickly convert string (nick) to int (fd)
		clients[fd].clientLog("Nickname set successfully, hello " + clientNick + ".", GRE);
		if (clients[fd].getStatus() == 1)
			clients[fd].incrementStatus();
		clients[fd].printLoginStatus();
		status = SUCCESS;
	}
}

void Server::user(int fd, std::istringstream& msg) {
	std::string clientUser;
	std::string clientHostname;
	std::string clientServername;
	std::string	clientRealname;
	status = SUCCESS;

	if (clients[fd].getStatus() == 0){
		clients[fd].clientLog("Missing password.", RED);
		clients[fd].printLoginStatus();
		return;
	}
	if (clients[fd].getStatus() == 1){
		clients[fd].clientLog("Missing nickname.", RED);
		clients[fd].printLoginStatus();
		return;
	}
	if (clients[fd].getStatus() > 2){
		clients[fd].clientLog("Your user has already been set up! It cannot be changed", RED);
		return;
	}
	msg >> clientUser >> clientHostname >> clientServername;
	std::getline(msg >> std::ws, clientRealname);
	if (clientUser.empty() || clientHostname.empty() ||\
		clientServername.empty() || clientRealname.empty() ||\
		clientRealname[0] != ':')
	{
			clients[fd].clientLog("Wrong syntax.", RED);
			clients[fd].printLoginStatus();
			return;
	}
	clientRealname.erase(0, 1);
	clients[fd].setUser(clientUser, clientHostname, clientServername, clientHostname);
	clients[fd].incrementStatus();
	clients[fd].clientLog("User set up succesfully, welcome!", GRE);
	status = SUCCESS;
}
