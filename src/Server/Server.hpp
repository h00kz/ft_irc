#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Channel.hpp"
#include "../Client.hpp"
#include <iostream>
#include <map>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sstream>
#include <fcntl.h>
#include <cerrno>
#include <csignal>

extern Server* g_server;

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
		Client		*findClient(const std::string &name);
		Channel		*findChannel(const std::string &name);

		/*			Pb_FUNCTIONS		 */
		void		Run();
		void		BroadcastMessage(const std::string& channel, const std::string& message, Client* sender);
		void		SendPrivateMessage(const std::string& target, const std::string& message, Client* sender);
		void 		RemoveChannel(const std::string &channel);
		void 		Close();

	private:
		/*			Pv_FUNCTIONS		 */
		void		DisconnectClient(Client* client, std::map<int, Client*>& clients);
		void 		HandleCommand(Client* client, const std::string& command, std::istringstream& iss);
		bool		HandleAuthentification(Client* client, const std::string& command, std::istringstream& iss);		

		//-----------CMD-----------------

		//User
		void    HandleUser(Client *client, std::istringstream &iss);

		//Pass
		void    HandlePass(Client *client, std::istringstream &iss);

		//Join
		void    HandleJoin(Client *client, std::istringstream &iss);

		//Ping
		void    HandlePing(Client *client);
		void	PingClients();

		//Pong
		void    HandlePong(Client *client);

		//PrivMsg
		void    HandlePrivMsg(Client *client, std::istringstream &iss);

		//Nick
		void    HandleNick(Client *client, std::istringstream &iss);
		bool    isAvailableNickname(std::string nickname);

		//Part
		void    HandlePart(Client *client, std::istringstream &iss);

		//List
		void   	HandleList(Client *client);

		//Mode
		void    HandleMode(Client *client, std::istringstream &iss);



		std::map<int, Client*>	_clients; // k: op_id, v: client
		std::map<std::string, Channel*>	_channels;
		sockaddr_in				_servAddr;
		std::string const		_serverPasswd;
		std::string const		_serverName;
		int						_serverSd;
		int						_port;
		int						_epollFd;
};

#endif
