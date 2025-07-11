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
	
	std::string coloredMsg = std::string(color) + msg + std::string(reset);
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
	return "";
}

bool isEmpty(std::istringstream &sstream)
{	std::string rest;

	sstream >> rest;
	if (rest.empty())
		return true;
	return false;
}