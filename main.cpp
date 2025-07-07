#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <cstdlib>
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
//-------------------------------------------------------//
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

static int validateInputPort(const std::string &inputPort)
{
	for (size_t i = 0; i < inputPort.size(); i++)
	{
		if (!std::isdigit(inputPort[i]))
		{
			std::cerr << "Input error! The port must be a number." << std::endl;
			exit(1);
		}
	}
	long portNum = std::atol(inputPort.c_str());
	if (portNum < 1024 || portNum > 65535)
	{
		std::cerr << "Input error! The port must be between 1024 and 65535." << std::endl;
		exit(1);
	}
	return (static_cast<int>(portNum));
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Input error! Usage: ./ircserv [port] [password]" << std::endl;
		return (1);
	}
	int port = validateInputPort(argv[1]);
	std::string pass(argv[2]);
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		ser.ServerInit(port, pass); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}

