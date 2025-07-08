
#ifndef SERVER_HPP
# define SERVER_HPP
enum CommandType {
	PASS,
	NICK,
	USER,
	JOIN,
	PRIVMSG
};
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <cstring>
#include <map>
#include <sstream>
//-------------------------------------------------------//
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color
#include "Client.hpp"

class Server //-> class for server
{
private:
	std::string password;
	int Port; //-> server port
	int SerSocketFd; //-> server socket file descriptor
	static bool Signal; //-> static boolean for signal
	std::map<int, Client> clients; //-> vector of clients
	std::vector<struct pollfd> fds; //-> vector of pollfd
public:
	Server(): SerSocketFd(-1) {} //-> default constructor

	void ServerInit(int port, const std::string &pass); //-> server initialization
	void SerSocket(); //-> server socket creation
	void AcceptNewClient(); //-> accept new client
	void ReceiveNewData(int fd); //-> receive new data from a registered client

	void ServerCommand();

	static void SignalHandler(int signum); //-> signal handler
	
	void parseInput(int fd, std::istringstream &strmMsg);
	void pass(int fd, std::istringstream& msg);
	void user(int fd, std::istringstream& msg);
	void nick(int fd, std::istringstream& msg);
	void privmsg(int fd, std::istringstream& msg);
	void join(int fd, std::istringstream& msg);
	void kick(int fd, std::istringstream& msg);
	void topic(int fd, std::istringstream& msg);
	void invite(int fd, std::istringstream& msg);
	void mode(int fd, std::istringstream& msg);
	void bet(int fd, std::istringstream& msg);
	void dcc(int fd, std::istringstream& msg);
    void commandLog(const std::string& command, bool status);

	bool isEmpty(std::istringstream &sstream);
	void CloseFds(); //-> close file descriptors
	void ClearClients(int fd); //-> clear clients
};

#endif