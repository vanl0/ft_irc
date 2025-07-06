#include "Server.hpp"

Server::Server(int const port) : _port(port){
    _serverSocketInit();
}

Server::~Server(){
}

void    Server::_serverSocketInit(void){
    //struct  sockaddr_in _server_addr;
    struct  pollfd      serPoll;

    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = INADDR_ANY;
    _server_addr.sin_port = htons(this->_port);

    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1)
		throw(std::runtime_error("failed to create socket"));
    if (fcntl(_server_fd, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("failed to set option (O_NONBLOCK) on socket"));
    if (bind(_server_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0)
        throw(std::runtime_error("failed to bind socket"));
    if (listen(_server_fd, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() failed"));

    serPoll.fd = _server_fd;
    serPoll.events = POLLIN;
    serPoll.revents = 0;
    _fds.push_back(serPoll);

    std::cout << "Server ready on port: " << _port << std::endl;
}

void    Server::serverInit(void){
    while (this->_signal == false){
        if((poll(&_fds[0],_fds.size(),-1) == -1) && this->_signal == false) //-> wait for an event
			throw(std::runtime_error("poll() failed"));
        for (size_t i = 0; i < _fds.size(); i++){ //-> check all file descriptors
			if (_fds[i].revents & POLLIN){ //-> check if there is data to read
				if (_fds[i].fd == _server_fd)
					_acceptNewClient(); //-> accept new client
				else
					_acceptClientInput(_fds[i].fd); //-> receive new data from a registered client
			}
        }
    }
    _closeFds();
}

void    Server::_acceptNewClient(void){
    //add new Client class to the vector
}

void    Server::_acceptClientInput(int fd){
    (void) fd;
    //parse n shit
}

void    Server::handleSignal(int sign){
    std::cout << "Signal: " << sign << " recieved" << std::endl;
    Server::_signal = true;
}

void    Server::_closeFds(void){
    for(size_t i = 0; i < _clients.size(); i++){ //-> close all the clients
		std::cout << "Client <" << _clients[i].getFd() << "> Disconnected" << std::endl;
		close(_clients[i].getFd());
	}
	if (_server_fd != -1){ //-> close the server socket
		std::cout << "Server <" << _server_fd << "> Disconnected"  << std::endl;
		close(_server_fd);
	}
}