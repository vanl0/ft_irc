#include "Server.hpp"
#include <algorithm>

Server::Server() : SerSocketFd(-1), status(SUCCESS) {};

void Server::ClearClients(int fd)
{
	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd)
		{
			fds.erase(fds.begin() + i);
			break;
		}
	}
	clients.erase(fd);

}

bool Server::Signal = false;

void Server::SignalHandler(int signum)
{
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true;
}

void Server::removeFromChannels(int fd)
{
	std::map<std::string, Channel>::iterator it;
	for (it = channels.begin(); it != channels.end(); ++it)
		it->second.removeUser(&clients[fd]);
}

void	Server::CloseFds()
{
	std::map<int, Client>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it)
	{
		std::cout << RED << "Client <" << it->first << "> Disconnected" << WHI << std::endl;
		removeFromChannels(it->first);
		close(it->first);
	}
	if (SerSocketFd != -1)
	{
		std::cout << RED << "Server <" << SerSocketFd << "> Disconnected" << WHI << std::endl;
		close(SerSocketFd);
	}
}

void Server::ReceiveNewData(int fd)
{
	char buff[1024];
	memset(buff, 0, sizeof(buff));

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
	buff[bytes] = '\0';
	std::string completeMsg = clients[fd].appendPartial(buff);
	if (bytes <= 0 || completeMsg.empty())
	{
		if (clients[fd].getStatus() > 1){
			std::cout << RED << "Client <" << clients[fd].getNick() << "> Disconnected" << WHI << std::endl;
			removeFromChannels(fd);
			nickFd.erase(clients[fd].getNick());
		} else
			std::cout << RED << "Client <fd: " << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd);
		close(fd);
	}
	else
	{
		completeMsg.erase(std::remove(completeMsg.begin(), completeMsg.end(), '\r'), completeMsg.end());
		size_t pos = completeMsg.find('\n');
		while (pos != std::string::npos){
			std::istringstream command(completeMsg.substr(0,pos));
			completeMsg = completeMsg.substr(pos + 1);
			parseInput(fd, command);
			pos = completeMsg.find('\n');
		}
		clients[fd].setPartial(completeMsg);
		//std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buff;
	}
}

void Server::AcceptNewClient()
{
	Client cli;
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);

	int incofd = accept(SerSocketFd, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return;
	}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
	{
		std::cout << "fcntl() failed" << std::endl;
		return;
	}

	NewPoll.fd = incofd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;

	cli.SetFd(incofd);
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr)));
	clients[incofd] = cli;
	fds.push_back(NewPoll);

	cli.clientLog(IRC_BANNER, MAG);
	cli.printLoginStatus();
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::ServerCommand()
{
	std::string line;
	std::getline(std::cin, line);
	if (line.empty())
		return;

	if (line == "exit")
	{
		Server::Signal = true;
		std::cout << "Shutting down server..." << std::endl;
		return;
	}
	// need to add private message commands on server side

	std::cout << "Unknown command: " << line << std::endl;
}

void Server::SerSocket()
{
	int en = 1;
	struct sockaddr_in add;
	struct pollfd NewPoll;
	add.sin_family = AF_INET;
	add.sin_addr.s_addr = INADDR_ANY;
	add.sin_port = htons(this->Port);

	SerSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (SerSocketFd == -1)
		throw(std::runtime_error("failed to create socket"));

	if (setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
	if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
	if (bind(SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1)
		throw(std::runtime_error("failed to bind socket"));
	if (listen(SerSocketFd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));

	NewPoll.fd = SerSocketFd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;
	fds.push_back(NewPoll);
}

void Server::ServerInit(int port, const std::string &pass)
{
	this->Port = port;
	this->password = pass;
	SerSocket();
	struct pollfd stdInPoll; // adding STDIN to the poll list for server commands
	stdInPoll.fd = STDIN_FILENO;
	stdInPoll.events = POLLIN;
	stdInPoll.revents = 0;
	if (fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on standard input"));
	fds.push_back(stdInPoll);

	std::cout << GRE << "IRC server <" << SerSocketFd << "> connected on port [" << this->Port << "]" << WHI << std::endl;
	std::cout << "Waiting to accept a new connection...\n";

	while (Server::Signal == false)
	{

		if ((poll(&fds[0],fds.size(),-1) == -1) && Server::Signal == false)
			throw (std::runtime_error("poll() failed"));

		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == STDIN_FILENO)
					ServerCommand(); //--> handle admin commands
				else if (fds[i].fd == SerSocketFd)
					AcceptNewClient();
				else
					ReceiveNewData(fds[i].fd);
			}
		}
	}
	CloseFds();
}
