#include "utils.hpp"

void validateInput(int argc, char *argv[])
{
	if (argc != 3)
		throw(std::runtime_error("[INPUT] Incorrect number of parameters"));
	std::string inputPort(argv[1]);
	std::string inputPass(argv[2]);
	for (size_t i = 0; i < inputPort.size(); i++)
	{
		if (!std::isdigit(inputPort[i]))
			throw(std::runtime_error("[INPUT] The port must be a number."));
	}
	long portNum = std::atol(inputPort.c_str());
	if (portNum < 1024 || portNum > 65535)
		throw(std::runtime_error("[INPUT] The port must be between 1024 and 65535."));
	if (inputPass.empty())
		throw(std::runtime_error("[INPUT] The password cannot be empty."));
}


void sendMsgFd(int fd, const std::string &msg)
{
	send(fd, msg.c_str(), msg.length(), 0);
}

void sendMsgFd(int fd, const std::string &msg, const char *color, const char *reset) 
{
	
	std::string coloredMsg = color + msg + reset;
	send(fd, coloredMsg.c_str(), coloredMsg.length(), 0);
}

const char *ansiToIrc(const std::string &ansi)
{
	if (ansi == RED) return (RED_HEX);
	if (ansi == GRE) return (GRE_HEX);
	if (ansi == YEL) return (YEL_HEX);
	if (ansi == BLU) return (BLU_HEX);
	if (ansi == MAG) return (MAG_HEX);
	if (ansi == WHI) return (WHI_HEX);
	if (ansi == RESET) return (RESET_HEX);
	if (ansi == RESETEND) return (RESET_HEXEND);
	return "";
}

bool isEmpty(std::istringstream &sstream)
{	std::string rest;

	sstream >> rest;
	if (rest.empty())
		return true;
	return false;
}

/*
https://datatracker.ietf.org/doc/html/rfc2812?#section-1.3
Channels names are strings (beginning with a '&', '#', '+' or '!') of up to 50 characters.
They cannot contain any spaces (' '), a control G (\a), a comma (',').
*/
bool isValidChannelName(const std::string &name)
{
	//std::cout << name << std::endl;
	if (name.empty() || name.size() > 50)
		return (false);
	if (name.find_first_of("&#+!") != 0)
		return (false);
	if (name.find_first_of(" \a,") != std::string::npos)
		return (false);
	return (true);
}

timeval getTime()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv;
}

std::string getTimeString() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    std::ostringstream oss;
    oss << tv.tv_sec; // usamos solo los segundos, ignoramos microsegundos
    return oss.str();
}

double secondsBetween(struct timeval start, struct timeval end)
{
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

std::string	skipStream(std::istringstream &iss){
	std::string next;
	if (!(iss >> next))
		return (std::string());
	return (next);
}

bool	isNumber(std::string &str){
	for (size_t i = 0; i < str.length(); i++){
		if (!std::isdigit(str[i]))
			return false;
	}
	return true;
}