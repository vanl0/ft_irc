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
    int port = 6667;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << port << std::endl;

    // Vector de pollfd: el primero es el socket servidor
    std::vector<struct pollfd> fds;
    struct pollfd server_pollfd;
    server_pollfd.fd = server_fd;
    server_pollfd.events = POLLIN;
    server_pollfd.revents = 0;
    fds.push_back(server_pollfd);

    char buffer[1024];

    while (true) {
        int ret = poll(&fds[0], fds.size(), -1);
        if (ret < 0) {
            perror("poll");
            break;
        }

        // Revisar todos los fds
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_fd) {
                    // Nuevo cliente conectado
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                    if (client_fd < 0) {
                        perror("accept");
                        continue;
                    }
                    std::cout << "New client connected: fd " << client_fd << std::endl;

                    struct pollfd client_pollfd;
                    client_pollfd.fd = client_fd;
                    client_pollfd.events = POLLIN;
                    client_pollfd.revents = 0;
                    fds.push_back(client_pollfd);
                } else {
                    // Cliente ya conectado envió datos
                    ssize_t bytes_read = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
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
                    } else {
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
