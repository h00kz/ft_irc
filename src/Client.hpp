#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <ctime>
# include <unistd.h>
# include <cstring>
# include <iostream>
# include <algorithm>
# include <map>

class Server;
class Channel;

class Client
{
	private:
		int _socketDescriptor;
		struct sockaddr_in _address;
		Server* _server;
		bool	_authenticated;
		std::string _receivedData;
		std::string _nickname;
		std::string _realName;
		std::string _hostName;
		std::string _serverName;
		std::string _username;
		std::map<std::string, Channel*>	_channels;
		time_t _lastActive;
	
	public:
		Client(int socketDescriptor, struct sockaddr_in address, Server* server);
		~Client();

		void	Close();
		void	JoinChannel(const std::string &channel);
		Channel	*FindChannel(const std::string &name);
		void	AddChannel(Channel* channel);
		void	LeaveChannels(void);
		void	LeaveChannel(Channel *channel);
		void	UpdateLastActive();
		int		ReceiveData();
		void	SendData(const std::string& data);
		void	SendMessage(const std::string &target, const std::string &message);
		
		//Setters

		void SetAuthenticated(bool b);
		void SetNickname(const std::string &nickname);
		void SetRealname(const std::string &nickname);
		void SetHost(const std::string &nickname);
		void SetUsername(const std::string &username);
		void SetServer(const std::string &username);
		
		//Getters

		const std::string 				&GetNickname() const;
		const std::string 				&GetRealname() const;
		const std::string 				&GetHost() const;
		std::string const 				&GetUsername() const;
		std::string const 				&GetServer() const;
		int 							GetSocketDescriptor() const;
		const std::string 				&GetReceivedData() const;
		time_t 							GetLastActive() const;
		std::map<std::string, Channel*>	&GetChannels();
		const struct sockaddr_in 		&GetAddress() const;
		bool							IsConnected() const;
		bool							IsAuthenticated() const;
		bool							IsInChannel(const std::string& channel) const;
};

#endif //CLIENT_HPP
