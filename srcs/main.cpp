#include "Server.hpp"

int main(int argc, char *argv[])
{
	Server ser;
	try
	{
		validateInput(argc, argv);
		int port = std::atoi(argv[1]);
		std::string pass(argv[2]);
		signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		ser.ServerInit(port, pass); //-> initialize the server
	}
	catch(const std::exception& e)
	{
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << "Error: " << e.what() << std::endl;
	}
	std::cout << "The IRC server closed!" << std::endl;
}

