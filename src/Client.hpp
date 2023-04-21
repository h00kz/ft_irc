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
	public:
		Client(int socketDescriptor, struct sockaddr_in address, Server* server);
		~Client();

		const struct sockaddr_in &GetAddress() const;

		void SetNickname(const std::string &nickname);
		const std::string &GetNickname() const;
		void SetRealname(const std::string &nickname);
		const std::string &GetRealname() const;
		void SetHost(const std::string &nickname);
		const std::string &GetHost() const;
		void SetUsername(const std::string &username);
		std::string const &GetUsername() const;
		void SetServer(const std::string &username);
		std::string const &GetServer() const;
		
		int GetSocketDescriptor() const;

		void JoinChannel(const std::string &channel);
		void LeaveChannel(const std::string &channel);
		Channel	*findChannel(const std::string &name);

		const std::string &GetReceivedData() const;
		int ReceiveData();
		time_t GetLastActive() const;
		void UpdateLastActive();

		void	enterChannel(const std::string& name, Channel *channel);
		bool IsInChannel(const std::string& channel) const;
		bool IsConnected() const;
		void Close();

		void SendData(const std::string& data);
		void SendMessage(const std::string &target, const std::string &message);

		void SetAuthenticated(bool b);
		bool IsAuthenticated();

		const std::map<std::string, Channel*>& GetChannels() const;
		void	AddChannel(Channel* channel);
		
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
};

#endif //CLIENT_HPP
