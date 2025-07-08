#include <sys/socket.h>
#include <string>
#include <iostream>
#include <cstdlib>

# define RED "\e[1;31m" //-> for red color
# define WHI "\e[0;37m" //-> for white color
# define GRE "\e[1;32m" //-> for green color
# define YEL "\e[1;33m" //-> for yellow color
# define BLU "\e[1;34m" //-> for blue color

void validateInput(int argc, char *argv[]);

void sendMsgFd(int fd, const std::string &msg);
void sendMsgFd(int fd, const std::string &msg, const char *color);
