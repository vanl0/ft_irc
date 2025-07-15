#include "Server.hpp"


void Server::parseInput(int fd, std::istringstream &strm_msg){
	int i;
	std::string command;
	std::string commands[] = {"PASS", "USER", "NICK", "PRIVMSG", "JOIN", "KICK", "TOPIC", "INVITE", "MODE"};
	strm_msg >> command;
	int len = sizeof(commands) / sizeof(commands[0]);
	// if (command == "l")
	// 	return log(fd, strm_msg);
	if (command == "CAP")
	{
		timeval now = getTime();
		double seconds = secondsBetween(clients[fd].getStart(), now);
		if (seconds < 0.1)
			return (clients[fd].setHexFlag());
	}
	for (i = 0 ; i < len ; i++){
		if (commands[i] == command)
			break;
	}
	if (i > 2 && clients[fd].getStatus() < 3){
		clients[fd].clientLog("You need to be registered first!\r\n", RED);
		clients[fd].printLoginStatus();
		if (i < 9)
			commandLog(command, FAIL);
		else
			commandLog(command, NOT_FOUND);
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
		default:
			status = NOT_FOUND;
	}
	commandLog(command, status);
}

/* void Server::invite(int fd, std::istringstream& msg) {
	std::string restOfMsg;
	std::getline(msg, restOfMsg);
	std::cout << "[INVITE] fd: " << fd << " msg:" << restOfMsg << std::endl;
} */




void Server::commandLog(const std::string& command, int status) {
	if (status == FAIL)
		std::cout << RED << "[INFO]: Execution of '" << command << "' failed" << WHI << std::endl;
	if (status == SUCCESS)
		std::cout << GRE << "[INFO]: Execution of '" << command << "' succeeded" << WHI << std::endl;
	if (status == NOT_FOUND)
		std::cout << RED << "[ERROR] Command: '" << command << "' not found" << WHI << std::endl;
}
