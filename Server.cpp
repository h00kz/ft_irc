#include "Server.hpp"
#include <cerrno>

Server::Server(int port, std::string const &passwd)
	: _serverPasswd(passwd), _serverName("MyServer"), _port(port)
{
	Init();
}

Command ParseCommand(const std::string& commandStr)
{
	if (commandStr == "NICK") {
		return NICK;
	} else if (commandStr == "USER") {
		return USER;
	} else if (commandStr == "JOIN") {
		return JOIN;
	} else if (commandStr == "PING") {
		return PING;
	} else if (commandStr == "PONG") {
		return PONG;
	} else if (commandStr == "PART") {
		return PART;
	} else if (commandStr == "PASS") {
		return PASS;
	} else if (commandStr == "PRIVMSG") {
		return PRIVMSG;
	} else {
		return UNKNOWN;
	}
}

void Server::HandleCommand(Client* client, const std::string& command, std::istringstream& iss)
{
	switch (ParseCommand(command)) {
		case PASS: {
			std::string password;
			iss >> password;
			std::cout << "PASS called\n";
			if (password == _serverPasswd)
				client->SetAuthenticated(true);
			else
			{
				std::cout << "Client not authenticated: " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
				client->Close();
				_clients.erase(client->GetSocketDescriptor());
				return;
			}
			break;
		}
		case NICK: {
			std::string nickname;
			iss >> nickname;
			client->SetNickname(nickname);
			std::cout << "NICK called\n";
			std::cout << "Client set nickname: " << nickname << std::endl;
			break;
		}
		case USER: {
			std::string username, host, server, realname;
			iss >> username >> host >> server;

			std::getline(iss, realname);

			client->SetUsername(username);
			client->SetHost(host);
			client->SetServer(server);
			client->SetRealname(realname);

			std::cout << "USER called\n";
			std::cout << "Client set username: " << username << std::endl;
			std::cout << "Client set host: " << host << std::endl;
			std::cout << "Client set server: " << server << std::endl;
			std::cout << "Client set real name: " << realname << std::endl;
			break;
		}
		case JOIN: {
			std::string channel;
			iss >> channel;
			client->JoinChannel(channel);
			std::cout << "Client joined channel: " << channel << std::endl;
			break;
		}
		case PING: {
			client->UpdateLastActive();
			std::string response = "PONG\r\n";
			client->SendData(response);
			std::cout << "PONG sent to " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
			break;
		}
		case PONG: {
			client->UpdateLastActive();
			std::cout << "PING recieve from " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
			break;
		}
		case PRIVMSG: {
			std::string target, message;
			iss >> target;
			getline(iss, message);
			// if (!message.empty() && message[0] == ' ') {
			// 	message.erase(0, 1);
			// }
			// if (!message.empty() && message[0] == ':') {
			// 	message.erase(0, 1);
			// }
			client->SendMessage(target, message);
			// std::cout << "Client sent message to " << target << ": " << message << std::endl;
			break;
		}
		case PART: {
			std::string channel;
			iss >> channel;
			client->LeaveChannel(channel);
			std::cout << "Client leave channel: " << channel << std::endl;
			break;
		}
		case UNKNOWN: {
			std::cout << "Unknown command: " << command << std::endl;
			break;
		}
	}
}

void Server::Close()
{
	if (_serverSd != -1) {
		close(_serverSd);
		_serverSd = -1;
	}
}
const std::map<int, Client*>& Server::GetClients() const
{
    return _clients;
}

void Server::RemoveChannel(const std::string &channel)
{
    std::vector<std::string>::iterator it = std::find(_channels.begin(), _channels.end(), channel);
    if (it != _channels.end())
    {
        _channels.erase(it);
        std::cout << "Channel removed: " << channel << std::endl;
    }
}

void Server::AddChannel(std::string const &channel)
{
	if (std::find(_channels.begin(), _channels.end(), channel) == _channels.end())
	{
		_channels.push_back(channel);
		std::cout << "Channel added: " << channel << std::endl;
	}
}

Server::~Server()
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		delete it->second;
	}
	close(_serverSd);
}

void Server::Init()
{
	_serverSd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSd == -1)
	{
    	std::cerr << "Socket: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	int optval = 1;
	if (setsockopt(_serverSd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
    	std::cerr << "Sockopt: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = INADDR_ANY;
	_servAddr.sin_port = htons(_port);
	if (bind(_serverSd, reinterpret_cast<struct sockaddr*>(&_servAddr), sizeof(_servAddr)) == -1)
	{
    	std::cerr << "Bind: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(_serverSd, 10) == -1)
	{
    	std::cerr << "Listen: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	fcntl(_serverSd, F_SETFL, O_NONBLOCK);
}

void Server::BroadcastMessage(const std::string& channel, const std::string& message, Client* sender)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		Client* client = it->second;
		if (client != sender && client->IsInChannel(channel))
			client->SendData(message);
	}
}

void Server::SendPrivateMessage(const std::string& target, const std::string& message, Client* sender)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it) {
		Client* client = it->second;
		if (client != sender && client->GetNickname() == target) 
			client->SendData(message);
	}
}

void Server::PingClients()
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		Client* client = it->second;
		if (client->IsConnected())
		{
			time_t currentTime = time(NULL);
			if (currentTime - client->GetLastActive() > 60)
			{
				std::string pingCommand = "PING\r\n";
				client->SendData(pingCommand);
				client->UpdateLastActive();
			}
		}
	}
}

void Server::DisconnectClient(std::map<int, Client*>::iterator& it, Client* client, std::map<int, Client*>& clients)
{
	if (client->IsConnected())
	{
		client->Close();
		delete client;
		clients.erase(it++);
	}
}

void Server::Run()
{
	int clientSd;
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	time_t timeout = 301;

	std::cout << "Server started on " << _port << " port .." << std::endl;

	while (true)
	{
		clientSd = accept(_serverSd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrSize);
		if (clientSd != -1)
		{
			fcntl(clientSd, F_SETFL, O_NONBLOCK);

			Client* newClient = new Client(clientSd, clientAddr, this);
			_clients[clientSd] = newClient;
			// newClient->SetAuthenticated(false);
			std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
		}

		std::map<int, Client*>::iterator it;
		for (it = _clients.begin(); it != _clients.end();)
		{
			Client* client = it->second;
			int bytesRead = client->ReceiveData();

			if (bytesRead == 0) // Client disconnected
			{
				std::cout << "Client disconnected: " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
				DisconnectClient(it, client, _clients);
			}
			else if (bytesRead > 0)
			{
				std::string receivedData = client->GetReceivedData();
				std::istringstream iss(receivedData);
				std::string command;
				// std::cout << receivedData << std::endl;
				while (iss >> command)
				{
					if (command == "PASS" || client->IsAuthenticated())
					{
						HandleCommand(client, command, iss);
						++it;
					}
					else
						DisconnectClient(it, client, _clients);
				}
			}
			else
				++it;
		}
		PingClients();
		time_t currentTime = time(NULL);
		for (it = _clients.begin(); it != _clients.end();)
		{
			
			Client* client = it->second;
			if (currentTime - client->GetLastActive() > timeout)
			{
				std::cout << "Client timeout: " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
				delete client;
				_clients.erase(it++);
			}
			else
				++it;
		}
	}
}