#include "Channel.hpp"
#include "Client.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <poll.h>
#include <vector>

int main() {
    int server_fd;
    struct sockaddr_in server_addr;
    int port = 6667; //must be > 1023 (0 to 1023 reserved)

    //socket(tcp protocol, type of stream, default) returns socket id
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    /*we activate reuseaddr so if we reset the 
    program quickly it doesnt show that the port we
    use is already occupied by the previous instance 
    since it takes some time for it to be set free 
    without reusaddr activated (opt = 1)*/
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    /*we clean server_addr struct from trash values*/
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;//default value
    server_addr.sin_addr.s_addr = INADDR_ANY;//listen to all ips
    server_addr.sin_port = htons(port);//port to connect

    /*we bind our socket structure to a real port socket in the computer,
    that way the port is binded to our program*/
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    /*We tell the program to start listening on the socket as a server socket, if theres more than 5
    simultanious connections, it will refuse the new ones*/
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;


    /*poll allows us to read from multiple connections, we create
    a vector of pollfds an we configure the server pollfd 
    which will be the first element*/
    std::vector<struct pollfd> fds;
    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;//indicate that the server socket looks for input to read
    server_pollfd.revents = 0; // init the flag that indicates if events (POLLIN) is true
    fds.push_back(server_pollfd);

    char buffer[1024];

    while (true) {
        int ret = poll(&fds[0], fds.size(), -1); //poll will wait till some event occurs
        if (ret < 0) {
            perror("poll");
            break;
        }

        // Revisar todos los fds
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) { //if flag activated for that socket
                if (fds[i].fd == server_fd) { //if revents flag is from the server means is a new client
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    //accept creates a new socket binded to the server port socket returns the fd created
                    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
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
                    fds.push_back(client_pollfd);
                } else {//already existing client sends data
                    ssize_t bytes_read = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);//read the message from the fd
                    if (bytes_read < 0) {
                        perror("recv");
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    } else if (bytes_read == 0) {
                        std::cout << "Client disconnected: fd " << fds[i].fd << std::endl;
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    } else {//print the msg
                        buffer[bytes_read] = '\0';
                        std::cout << "Received from fd " << fds[i].fd << ": " << buffer << std::endl;
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < fds.size(); ++i) {
        close(fds[i].fd);
    }

    return 0;
}
