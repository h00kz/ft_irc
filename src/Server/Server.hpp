#ifndef SERVER_HPP
# define SERVER_HPP

# include "../Channel.hpp"
# include "../Client.hpp"
# include <iostream>
# include <map>
# include <vector>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <arpa/inet.h>
# include <cstdlib>
# include <cstring>
# include <unistd.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sstream>
# include <fcntl.h>
# include <cerrno>
# include <csignal>

# define CHANNEL_LIMIT 10

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
	MODE,
	TOPIC,
	INVITE,
	KICK
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
		void		JoinChannel(Client* client, const std::string& chanName);
		void		CloseEmptyChannels(void);

		/*			Pb_FUNCTIONS		 */
		void		Run();
		void		BroadcastMessage(const std::string& channel, const std::string& message, Client* sender);
		void		SendPrivateMessage(const std::string& target, const std::string& message, Client* sender);
		void 		RemoveChannel(const std::string &channel);
		void 		Close();

	private:
		/*			Pv_FUNCTIONS		 */
		bool		DisconnectClient(Client* client, std::map<int, Client*>& clients);
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
		void HandleInviteMode(Client *client, Channel *channel, char operation);
		void HandleTopicMode(Client *client, Channel *channel, char operation);
		void HandleKeyMode(Client *client, Channel *channel, std::istringstream &iss, char operation);
		void HandleOperatorMode(Client *client, Channel *channel, std::istringstream &iss, char operation);
		void HandleLimitMode(Client *client, Channel *channel, std::istringstream &iss, char operation);


		//Topic
		void    HandleTopic(Client *client, std::istringstream &iss);

		//Invite
		void    HandleInvite(Client *client, std::istringstream &iss);

		//Kick
		void	HandleKick(Client *client, std::istringstream &iss);

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
