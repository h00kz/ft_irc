#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sstream>
#include <fcntl.h>
#include <cerrno>
#include <csignal>

enum Command {
	NICK,
	USER,
	JOIN,
	PING,
	PONG,
	PART,
	PASS,
	PRIVMSG,
	UNKNOWN,
	LIST,
	MODE
};

class Server
{
	public:
		/*			CONSTRUCTORS/DES		 */
					Server();
					Server(int port, std::string const &passwd);
					~Server();

		/*			ACCESSORS			 */
		std::map<int, Client*> const&	GetClients() const;
	
		/*			Pb_FUNCTIONS		 */
		void		Run();
		void		BroadcastMessage(const std::string& channel, const std::string& message, Client* sender);
		void		SendPrivateMessage(const std::string& target, const std::string& message, Client* sender);
		void 		RemoveChannel(const std::string &channel);
//		void		AddChannel(std::string const& channel);
		void 		Close();

	private:
		/*			Pv_FUNCTIONS		 */
		void		Init();
		void		PingClients();
		void		DisconnectClient(std::map<int, Client*>::iterator& it, Client* client, std::map<int, Client*>& clients);
		void 		HandleCommand(Client* client, const std::string& command, std::istringstream& iss);
		void		HandleAuthentification(Client* client, const std::string& command, std::istringstream& iss);		
		int			findChannel(std::string channel);
		void		JoinChannel(Client *client, std::string channel);
		Channel		*createChannel(std::string name, Client *client);


		std::map<int, Client*>	_clients; // k: op_id, v: client
		std::vector<Channel*>	_channels;
		sockaddr_in				_servAddr;
		std::string const		_serverPasswd;
		std::string const		_serverName;
		int						_serverSd;
		int						_port;
		int						_epollFd;

};

#endif