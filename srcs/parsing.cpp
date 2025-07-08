#include "Server.hpp"

void Server::parseInput(int fd, std::istringstream &strm_msg){
	int i;
	std::string command;
	std::string commands[] = {"PASS", "USER", "NICK", "PRIVMSG", "JOIN", "KICK", "TOPIC", "INVITE", "MODE", "BET", "DCC", "PING"};
	strm_msg >> command;
	int len = sizeof(commands) / sizeof(commands[0]);
	// if (command == "l")
	// 	return log(fd, strm_msg);
	for (i = 0 ; i < len ; i++){
		if (commands[i] == command)
			break;
	}
	if (i > 2 && clients[fd].getStatus() < 3){
		clients[fd].clientLog("You need to be registered first!\n");
		clients[fd].printLoginStatus();
		return;
	}
	switch (i)
	{
		case (0):
			pass(fd, strm_msg);
			break;
		case (1):
			user(fd, strm_msg);
			break;
		case (2):
			nick(fd, strm_msg);
			break;
		case (3):
			privmsg(fd, strm_msg);
			break;
		case (4):
			join(fd, strm_msg);
			break;
		case (5):
			kick(fd, strm_msg);
			break;
		case (6):
			topic(fd, strm_msg);
			break;
		case (7):
			invite(fd, strm_msg);
			break;
		case (8):
			mode(fd, strm_msg);
			break;
		case (9):
			bet(fd, strm_msg);
			break;
		case (10):
			dcc(fd, strm_msg);
			break;
		case (11):
			commandLog("PING", true);
			break;
		default:
			commandLog(command, false);
			//clientLog(fd, "Bad input\n");
	}
}



void Server::privmsg(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[PRIVMSG] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::join(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[JOIN] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::kick(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[KICK] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::topic(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[TOPIC] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::invite(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[INVITE] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::mode(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[MODE] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::bet(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[BET] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::dcc(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[DCC] fd: " << fd << " msg:" << restOfMsg << std::endl;
}

void Server::commandLog(const std::string& command, bool status) {
    std::cout << "[LOG] Command: " << command << " Status: " << (status ? "OK" : "FAIL") << std::endl;
}
