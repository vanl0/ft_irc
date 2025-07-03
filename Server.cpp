#include "Server.hpp"

Server::Server(int const port): _running(true), _port(port){
	
	_serverSocketInit();
	std::cout << "Server listening on port " << port << std::endl;

    _server_pollfd.fd = _server_fd;
    _server_pollfd.events = POLLIN;
    _server_pollfd.revents = 0;
    _fds.push_back(_server_pollfd);

	while (_running) {
        int ret = poll(&_fds[0], _fds.size(), -1); //poll will wait till some event occurs
        if (ret < 0) {
            perror("poll");
			throw std::runtime_error("poll error");
            break;
        }

        for (size_t i = 0; i < _fds.size(); ++i) {
            if (_fds[i].revents & POLLIN) { //if flag activated for that socket
                if (_fds[i].fd == _server_fd) { //if revents flag is from the server means is a new client
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    //accept creates a new socket binded to the server port socket returns the fd created
                    int client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd < 0) {
                        perror("accept");
                        continue;
                    }
                    std::cout << "New client connected: fd " << client_fd << std::endl;
                    //we add the new client to the pollfd list
                    struct pollfd client_pollfd;
                    client_pollfd.fd = client_fd;
                    client_pollfd.events = POLLIN;
                    client_pollfd.revents = 0;
                    _fds.push_back(client_pollfd);
                } else {//already existing client sends data
                    ssize_t bytes_read = recv(_fds[i].fd, _buffer, sizeof(buffer) - 1, 0);//read the message from the fd
                    if (bytes_read < 0) {
                        perror("recv");
                        close(_fds[i].fd);
                        _fds.erase(_fds.begin() + i);
                        --i;
                    } else if (bytes_read == 0) {
                        std::cout << "Client disconnected: fd " << _fds[i].fd << std::endl;
                        close(_fds[i].fd);
                        _fds.erase(_fds.begin() + i);
                        --i;
                    } else {//print the msg
                        buffer[bytes_read] = '\0';
                        std::cout << "Received from fd " << _fds[i].fd << ": " << buffer << std::endl;
                    }
                }
            }
        }
    }

}

Server::~Server(void){
	for (size_t i = 0; i < _fds.size(); ++i) {
        close(_fds[i].fd);
    }
	std::cout << "Server finished succesfully" << std::endl;
}

void	Server::_serverSocketInit(void){
	this->_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd < 0) {
        perror("socket");
        throw Server::ServerInitError();
    }

	int opt = 1;
    setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&_server_addr, 0, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;//default value
    _server_addr.sin_addr.s_addr = INADDR_ANY;//listen to all ips
    _server_addr.sin_port = htons(_port);//port to connec

    if (bind(_server_fd, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) < 0) {
        perror("bind");
        close(_server_fd);
		throw Server::ServerInitError();
    }

	if (listen(_server_fd, 5) < 0) {
        perror("listen");
        close(_server_fd);
        throw Server::ServerInitError();
    }
}

const char *Server::ServerInitError::what(void) const throw(){
	return ("Error occurred during server start");
}