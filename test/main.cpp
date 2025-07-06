#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"

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
    Server myserv(6667);

    return 0;
}
