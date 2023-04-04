#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <algorithm>

class Server;

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
		
		const std::vector<std::string> &GetChannels() const;
		
		int GetSocketDescriptor() const;

		void JoinChannel(const std::string &channel);
		void LeaveChannel(const std::string &channel);
		const std::string &GetReceivedData() const;
		int ReceiveData();
		time_t GetLastActive() const;
		void UpdateLastActive();

		bool IsInChannel(const std::string& channel) const;
		bool IsConnected() const;
		void Close();

		void SendData(const std::string& data);
		void SendMessage(const std::string &target, const std::string &message);

		void SetAuthenticated(bool b);
		bool IsAuthenticated();
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
		std::vector<std::string> _channels;
		time_t _lastActive;
};

#endif
