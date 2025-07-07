
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <poll.h>
#include <vector>
#include <fcntl.h>

class   Server;

class Client //-> class for client
{
private:
	int 		Fd;
	std::string IPadd;
	std::string	messageBuffer;
	int			loginStatus;

	std::string	nick;
	std::string user[4];
public:
	Client(){loginStatus = 0;};

	int GetFd(){return Fd;}
	int getStatus(void) const;

//---LOGIN---------------------------------------
	void		setNick(std::string const &nick);
	std::string getNick(void) const;
	void		setUser(std::string const &username, std::string const &hostName, std::string const &serverName, std::string const &realName);
	std::string	getUser(int i) const;
	void		printLoginStatus(void) const;

	void SetFd(int fd){Fd = fd;}
	void setIpAdd(std::string ipadd){IPadd = ipadd;}
	void incrementStatus(void);
	void clientLog(std::string const &msg) const;
	void clientLog(std::string const &msg, const char* color) const;
	std::string	readMessage();
};

#endif