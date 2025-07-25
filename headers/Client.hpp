
#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "utils.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <iostream>
# include <cstring>
# include <cstdio>
# include <poll.h>
# include <vector>
# include <fcntl.h>

# define SERV_NAME "ircserv :"
class   Server;

class Client
{
private:
	int 		Fd;
	std::string IPadd;
	std::string	messageBuffer;
	int			loginStatus;
	bool		hexFlag;

	std::string	nick;
	std::string user[4];

	std::string _partialMsg;

	timeval 	start;
public:
	Client();
	int 		GetFd() const;
	int 		getStatus(void) const;
	timeval		getStart(void) const;
	bool 		getHexFlag() const;

//---LOGIN---------------------------------------
	void		setNick(std::string const &nick);
	std::string getNick(void) const;
	void		setUser(std::string const &username, std::string const &hostName, std::string const &serverName, std::string const &realName);
	std::string	getUser(int i) const;
	void		printLoginStatus(void) const;

	std::string readMessage(int const fd);
	std::string	appendPartial(char *buffer);
	void		setPartial(std::string const &part);

	void 		SetFd(int fd);
	void 		setIpAdd(std::string ipadd);
	void 		setHexFlag();
	void 		incrementStatus(void);
	void 		clientLog(std::string const &msg) const;
	void 		clientLog(std::string const &msg, const char* color) const;
	void		clientLog(const std::string &msg, const char *color, bool end) const;
};

#endif