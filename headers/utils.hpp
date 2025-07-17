
#ifndef UTILS_HPP
# define UTILS_HPP
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <sys/time.h>

# define RED		"\e[1;31m" //-> for red color
# define GRE		"\e[1;32m" //-> for green color
# define YEL		"\e[1;33m" //-> for yellow color
# define BLU		"\e[1;34m" //-> for blue color
# define MAG		"\e[1;35m" // magenta
# define WHI		"\e[0;37m" //-> for white color
# define RESET		"\e[0m"

# define RED_HEX	"\x03""04" // Red
# define GRE_HEX	"\x03""03" // Green
# define YEL_HEX	"\x03""08" // Yellow
# define BLU_HEX	"\x03""02" // Blue
# define MAG_HEX	"\x03""06" // Magenta (purple)
# define WHI_HEX	"\x03""00" // White (default)
# define BOLD_HEX	"\x02"     // Bold
# define RESET_HEX	"\x0F"     // Reset formatting

void		validateInput(int argc, char *argv[]);

void		sendMsgFd(int fd, const std::string &msg);
void		sendMsgFd(int fd, const std::string &msg, const char *color, const char *reset);

const char	*ansiToIrc(const std::string &ansi);

bool		isEmpty(std::istringstream &sstream);
bool		isValidChannelName(const std::string &name);

timeval		getTime();
double		secondsBetween(struct timeval start, struct timeval end);
std::string getTimeString();

std::string skipStream(std::istringstream &iss);
bool		isNumber(std::string &str);
#endif