#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <poll.h>
#include <vector>
#include <csignal>
bool Server::_signal = false;

int main()
{
	Server ser(6667);
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::handleSignal); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::handleSignal); //-> catch the signal (ctrl + \)
		ser.serverInit(); //-> initialize the server
	}
	catch(const std::exception& e){
		ser._closeFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}