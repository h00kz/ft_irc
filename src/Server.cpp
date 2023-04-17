#include "Server.hpp"

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
	switch (ParseCommand(command))
	{
		case PASS: {
			client->SendData("Error you may not reregister\n");
			return ;
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
			if (!realname.empty() && realname[0] == ':')
				realname.erase(0, 1);
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
			// client->JoinChannel(channel);
			this->JoinChannel(client, channel);
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
			if (!message.empty() && message[0] == ':')
				message.erase(0, 1);
			client->SendMessage(target, message);
			std::cout << "Client sent message to " << target << ": " << message << std::endl;
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
			if (client->GetChannels().size() != 0)
			{
				std::cout << client->GetChannels().at(0) << " " << client->GetNickname() << ": " << command << std::endl;
			}
			else
				std::cout << "Unknown command: " << command << std::endl;
			break;
		}
	}
}

void Server::Close()
{
	if (_serverSd != -1)
	{
		std::map<int, Client*>::iterator it = _clients.begin();
		for (;it != _clients.end();)
			DisconnectClient(it, it->second, _clients);
		std::cout << std::endl << "_clients size : " << _clients.size()<< " content(sockD) : " << std::endl;
		close(_serverSd);
		close(_epollFd);
		_serverSd = -1;
	}
}

const std::map<int, Client*>& Server::GetClients() const
{
	return _clients;
}

void Server::RemoveChannel(const std::string &channel)
{
	int	channel_pos = findChannel(channel);

	if (channel_pos != -1)
		this->_channels.erase(this->_channels.begin() + channel_pos);
	else
		std::cout << "This channel does not exist" << std::endl;
}

/*
void Server::AddChannel(std::string const &channel, )
{
	if (findChannel(channel) == -1)
	{
		_channels.push_back(channel);
		std::cout << "Channel added: " << channel << std::endl;
	}
}
*/

int	Server::findChannel(std::string channel)
{
	int	i = 0;

	for (std::vector<Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        if ((*it)->getName() == channel)
            return (i);
		i++;
	}
    return (-1);
}

Channel    *Server::createChannel(std::string name, Client *client)
{
	Channel	*channel = new Channel(name, client);
	return (channel);
}

void		Server::JoinChannel(Client *client, std::string channel)
{
	int channel_pos = this->findChannel(channel);
	if (channel_pos == -1)
		this->_channels.push_back(createChannel(channel, client));
	else
		this->_channels.at(channel_pos)->addClient(client);
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
	_epollFd = epoll_create1(0);
	if (_epollFd == -1)
	{
		std::cerr << "Epoll create: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = _serverSd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSd, &ev) == -1)
	{
		std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::BroadcastMessage(const std::string& channel, const std::string& message, Client* sender)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* client = it->second;
		if (client != sender && client->IsInChannel(channel))
			client->SendData(message);
	}
}

void Server::SendPrivateMessage(const std::string& target, const std::string& message, Client* sender)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
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
		if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
			std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
		std::cout << "Client disconnected: " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
		client->Close();
		delete client;
		clients.erase(it++);
	}
}

void Server::HandleAuthentification(Client* client, const std::string& command, std::istringstream& iss)
{
	switch (ParseCommand(command))
	{
		case PASS:
		{
			std::string password;
			iss >> password;
			std::cout << "PASS called\n";
			if (password == _serverPasswd && !client->IsAuthenticated())
			{
			 	client->SetAuthenticated(true);
				client->SendData("u find the pass\n");
			}
			else if (client->IsAuthenticated())
			{
				client->SendData("Error you may not reregister\n");
				return ;
			}
			else if (password == "USER" || password == "")
			{
				client->SendData("Error PASS not enough parameters\n");
				if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
					std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
				client->Close();
				_clients.erase(client->GetSocketDescriptor());
				return ;
			}
			else
			{
				std::cout << "Client not authenticated: " << inet_ntoa(client->GetAddress().sin_addr) << " sock: " << client->GetSocketDescriptor() << std::endl;
				if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
					std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
				client->Close();
				_clients.erase(client->GetSocketDescriptor());
				return;
			}
			break;
		}
		case USER:
		{
			if (client->IsAuthenticated())
			{
				std::string username, host, server, realname;
				iss >> username >> host >> server;

				std::getline(iss, realname);
				if (!realname.empty() && realname[0] == ':')
					realname.erase(0, 1);
				client->SetUsername(username);
				client->SetHost(host);
				client->SetServer(server);
				client->SetRealname(realname);
				client->SendData("good username\n");

				std::cout << "USER called\n";
				std::cout << "Client set username: " << username << std::endl;
				std::cout << "Client set host: " << host << std::endl;
				std::cout << "Client set server: " << server << std::endl;
				std::cout << "Client set real name: " << realname << std::endl;

				break;
			}
			else
			{
				std::cout << "Client not authenticated: " << inet_ntoa(client->GetAddress().sin_addr) << " sock: " << client->GetSocketDescriptor() << std::endl;
				if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
					std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
				client->Close();
				_clients.erase(client->GetSocketDescriptor());
				return;
			}
		}
			default : 
		{
			std::cout << "Client not authenticated: " << inet_ntoa(client->GetAddress().sin_addr) << " sock: " << client->GetSocketDescriptor() << std::endl;
			if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
				std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
			client->Close();
			_clients.erase(client->GetSocketDescriptor());
			return;
		break;
		}
	}
}

void Server::Run()
{
	int clientSd;
	const int MAX_EVENTS = 64;
	struct epoll_event events[MAX_EVENTS];
	struct sockaddr_in clientAddr;
	socklen_t clientAddrSize = sizeof(clientAddr);
	time_t timeout = 301;
	

	std::cout << "Server started on " << _port << " port .." << std::endl;

	while (true)
	{
		int nfds = epoll_wait(_epollFd, events, MAX_EVENTS, 500);
		if (nfds == -1)
		{
			std::cerr << "Epoll_wait: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < nfds; ++i)
		{
			if (events[i].data.fd == _serverSd)
			{
				clientSd = accept(_serverSd, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrSize);
				if (clientSd != -1)
				{
					struct epoll_event ev;
					bzero(&ev, sizeof(ev));
					ev.events = EPOLLIN;
					ev.data.fd = clientSd;
					fcntl(clientSd, F_SETFL, O_NONBLOCK);
					Client* newClient = new Client(clientSd, clientAddr, this);
					_clients[clientSd] = newClient;
					std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
					if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientSd, &ev) == -1)
					{
						std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
						exit(EXIT_FAILURE);
					}
				}
			}
			else
			{
				std::map<int, Client*>::iterator it;
				for (it = _clients.begin(); it != _clients.end();)
				{
					Client* client = it->second;
					int bytesRead = client->ReceiveData();

					if (bytesRead == 0) // Client disconnected
						DisconnectClient(it, client, _clients);
					else if (bytesRead > 0)
					{
						std::string receivedData = client->GetReceivedData();
						std::istringstream iss(receivedData);
						std::string command;
						std::cout << receivedData << std::endl; // PRINT DATA REQUEST CLIENT
						while (iss >> command)
						{
							if (_clients.size() > 0)
							{
								if (client->IsAuthenticated() && client->GetUsername() != "")
								{
									HandleCommand(client, command, iss);
									++it;
								}
								else
								{
									std::cout << command;
									HandleAuthentification(client, command, iss);
								}
							}
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
	}
}
