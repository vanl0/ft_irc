
#ifndef SERVER_HPP
# define SERVER_HPP

# include "utils.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include <iostream>
# include <cstdlib>
# include <map>
# include <vector> //-> for vector
# include <sys/socket.h> //-> for socket()
# include <sys/types.h> //-> for socket()
# include <netinet/in.h> //-> for sockaddr_in
# include <fcntl.h> //-> for fcntl()
# include <unistd.h> //-> for close()
# include <arpa/inet.h> //-> for inet_ntoa()
# include <poll.h> //-> for poll()
# include <csignal> //-> for signal()

#define IRC_BANNER \
"             )    \n" \
"             \\   )   \n" \
"             ()  \\                           )\n" \
"                 ()                       )  \\\n" \
"                       .-\"\"\"-.            \\  ()\n" \
"              ____    /  __   `\\     __   ()\n" \
"           .'`  __'. | o/__\\o   |   / /|\n" \
"          /  o /__\\o;\\  \\\\//   /_  // /\n" \
" ._      _|    \\\\// |`-.__.-'|\\  `;  /\n" \
"/  \\   .'  \\-.____.'|   ||   |/    \\/\n" \
"`._ '-/             |   ||   '.___./\n" \
".  '-.\\_.-'      __.'-._||_.-' _ /\n" \
".`\"\"===(||).___.(||)(||)----'(||)===...__\n" \
" `\"===\"`\"\"=====\"\"\"\"========\"\"\"====...__  `\"\"==._\n" \
"                                       `\"=.     `\"=.\n" \
"                                           `\"=.\n" \
"       Welcome to the ft_irc!\n" \
"    Made by pde-masc & ilorenzo"


enum CommandType {
	PASS,
	NICK,
	USER,
	JOIN,
	PRIVMSG
};

enum CommandReturn{
	FAIL,
	SUCCESS,
	NOT_FOUND
};

class Server
{
private:
	std::string						password;
	int 							Port;
	int 							SerSocketFd;
	static bool						Signal;

	std::map<int, Client>			clients;
	std::vector<struct pollfd>		fds;

	std::map<std::string, Channel>	channels;
	std::map<std::string, int>		nickFd;

	int	status;
public:
	Server();

	void		ServerInit(int port, const std::string &pass);
	void		SerSocket(void);
	void 		AcceptNewClient();
	void 		ReceiveNewData(int fd);

	void 		ServerCommand();

	static void	SignalHandler(int signum);

	void 		parseInput(int fd, std::istringstream &strmMsg);
	void 		pass(int fd, std::istringstream& msg);
	void 		user(int fd, std::istringstream& msg);
	void 		nick(int fd, std::istringstream& msg);
	void 		privmsg(int fd, std::istringstream& msg);
	void 		privmsg(int fd, std::string& msg, std::string& chName);
	void 		join(int fd, std::istringstream& msg);
	void 		kick(int fd, std::istringstream& msg);
	void 		topic(int fd, std::istringstream& msg);
	void 		invite(int fd, std::istringstream& msg);
	void 		mode(int fd, std::istringstream& msg);
	void 		parseMode(int fd, const std::string &flags, std::istringstream &msg, Channel *channel, std::string &params, bool plusminus);
	void 		commandLog(const std::string& command, int status);

	bool 		nickInUse(const std::string &nick);

	void 		removeFromChannels(int fd);
	void 		CloseFds();
	void 		ClearClients(int fd);
};

#endif