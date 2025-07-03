#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <poll.h>
#include <vector>

class	Server{

private:
	int const					_port;
	int							_server_fd;
	struct sockaddr_in			_server_addr;
	struct pollfd				_server_pollfd;
	std::vector<struct pollfd>	_fds;
	char						*_buffer[1024];

	bool						_running;
	void						_serverSocketInit(void);
public:
	Server(int const port);
	~Server(void);

	class ServerInitError : public std::exception {
			public :
					const char *	what (void) const throw();
		};

};