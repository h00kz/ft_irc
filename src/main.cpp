#include "Server.hpp"
#include <csignal>

Server* g_server = NULL;

void mhandleSigint(int signal)
{
	if (g_server != NULL)
		g_server->Close();
	std::cout << "\nTerminating the server." << std::endl;
	exit(signal);
}

int main(int argc, char const *argv[])
{
	signal(SIGINT, mhandleSigint);
	if (argc != 3)
	{
		std::cout << "Usage: ./irc <Port> <Password>" << std::endl;
	}
	else
	{
		int port = std::atoi(argv[1]);
		if (port <= 0 || port != static_cast<uint16_t>(port))
		{
			std::cerr << "Invalid port number. Please provide a valid port number between 1 and 65535." << std::endl;
			return 1;
		}
		Server server(port, argv[2]);
		g_server = &server;
		server.Run();
	}
	return 0;
}
