
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

class   Client{

private:
    bool    _isRegistered;

    std::string _name;
    std::string _nick;

    int         _fd;

    //std::vector<Channel>    _channels;


public:
    Client();
    ~Client();

    std::string getName(void) const;
    std::string getNick(void) const;
    int         getFd(void) const;
};

#endif