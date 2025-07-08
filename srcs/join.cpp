#include "Server.hpp"

void Server::join(int fd, std::istringstream& msg) {
    std::string restOfMsg;
    std::getline(msg, restOfMsg);
    std::cout << "[JOIN] fd: " << fd << " msg:" << restOfMsg << std::endl;
}