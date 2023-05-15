
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:31:13 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/06 18:46:24 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static volatile bool quitStatus = false;

void shandleSigint(int signal)
{
	std::cout << "\nTerminating the server." << std::endl;
	quitStatus = true;
	g_server->Close();
	exit(signal);
}

Server::Server(int port, std::string const &passwd)
	: _serverPasswd(passwd), _serverName("MyServer"), _port(port)
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
	} else if (commandStr == "LIST") {
		return LIST;
	} else if (commandStr == "TOPIC") {
		return TOPIC;
	} else if (commandStr == "INVITE") {
		return INVITE;
	} else if (commandStr == "KICK") {
		return KICK;
	} else if (commandStr == "MODE") {
		return MODE;
	} else if (commandStr == "NOTICE") {
		return NOTICE;
	} else {
		return UNKNOWN;
	}
}

void Server::HandleCommand(Client* client, std::string& command, std::istringstream& iss)
{
	switch (ParseCommand(command))
	{
		case PASS: {
			HandlePass(client, iss);
			break;
		}
		case NICK: {
			HandleNick(client, iss);
			break;
		}
		case USER: {
			HandleUser(client, iss);
			break;
		}
		case JOIN: {
			HandleJoin(client, iss);
			break;
		}
		case PING: {
			HandlePing(client, iss);
			break;
		}
		case PONG: {
			HandlePong(client);
			break;
		}
		case PRIVMSG: {
			HandlePrivMsg(client, iss);
			break;
		}
		case PART: {
			HandlePart(client, iss);
			break;
		}
		case LIST: {
			HandleList(client);
			break;
		}
		case TOPIC: {
			HandleTopic(client, iss);
			break;
		}
		case INVITE: {
			HandleInvite(client, iss);
			break;
		}
		case KICK: {
			HandleKick(client, iss);
			break;
		}
		case MODE: {
			HandleMode(client, iss);
			break;
		}
		case NOTICE: {
			HandleNotice(client, iss);
			break;
		}	
		case UNKNOWN: {
			std::cout << "Unknown command: " << command << std::endl;
			while(iss.get() != '\n');
			break;
		}
	}
	client->clearCmd();
}

bool Server::HandleAuthentification(Client* client, std::string& command, std::istringstream& iss)
{
	if (command != "PASS" && client->IsAuthenticated() == false)
	{
		DisconnectClient(client, _clients);
		return false;
	}
	switch (ParseCommand(command))
	{
		case PASS:
		{
			HandlePass(client, iss);
			break;
		}
		case NICK:
		{
			HandleNick(client, iss);
			break;
		}
		case USER:
		{
			HandleUser(client, iss);
			break;
		}
		default :
		{
			client->SendData("You're not fully authenticated :\n");
			if (client->GetNickname() == "")
				client->SendData("Nickname must be set\n");
			if (client->GetUsername() == "")
				client->SendData("Username must be set\n");
			while(iss.get() != '\n');
		}
	}
	client->clearCmd();
	return true;
}

void Server::Close()
{
	if (_serverSd != -1)
	{
		std::map<int, Client*>::iterator it = _clients.begin();
		for (;it != _clients.end();)
		{
			if (DisconnectClient(it->second, _clients))
				it = _clients.begin();
		}
		close(_serverSd);
		close(_epollFd);
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

Client	*Server::FindClient(const std::string &name)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        Client* client = it->second;
        if (client->GetNickname() == name)
            return (client);
    }
    return (NULL);
}

Channel	*Server::FindChannel(const std::string &name)
{
    std::map<std::string, Channel *>::iterator it = _channels.find(name);
    if (it != _channels.end())
        return (it->second);
    return (NULL);
}

void Server::RemoveChannel(const std::string &name)
{
	Channel *channel;

	channel = FindChannel(name);
	if (channel != NULL)
	{
		delete (channel);
		this->_channels.erase(name);
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

void Server::SendPrivateMessage(const std::string& target, const std::string& message, Client* sender, bool error_notifications)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client* client = it->second;
		if (client != sender && client->GetNickname() == target) 
		{
			client->SendData(message);
			break;
		}
	}
	if (it == _clients.end() && error_notifications == true)
		sender->SendData("PRIVMSG :This user does not exist\n");
}

bool Server::DisconnectClient(Client* client, std::map<int, Client*>& clients)
{
	if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
		std::cerr << "Epoll_ctl DISCO: " << strerror(errno) << std::endl;
	std::cout << "Client disconnected: " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
	client->Close();
	clients.erase(client->GetSocketDescriptor());
	client->LeaveChannels();
	CloseEmptyChannels();
	delete client;
	return true;
}

void	Server::CloseEmptyChannels(void)
{
	std::vector<std::string> emptyChannels;
	Channel	*tmp;

	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		std::cout << it->second->GetNbClients() << " clients left in the channel " << it->second->GetName() << std::endl;

		if (it->second->GetNbClients() == 0) {
			emptyChannels.push_back(it->first);
		}
	}
	for (size_t i = 0; i < emptyChannels.size(); i++)
	{
		tmp = _channels.find(emptyChannels[i])->second;
		_channels.erase(emptyChannels[i]);
		std::cout << tmp->GetName() << " channel deleted\n";
		delete tmp;
	}
	emptyChannels.clear();
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
	while (quitStatus != true)
	{
		std::signal(SIGINT, shandleSigint);
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
						Close();
						exit(EXIT_FAILURE);
					}
				}
			}
			else
			{
				std::map<int, Client*>::iterator it;
				for (it = _clients.begin(); it != _clients.end();)
				{
					if (!_clients.empty())
					{
						Client* client = it->second;
						int bytesRead = client->ReceiveData();
						if (bytesRead == 0) // Client disconnected
						{
							if (DisconnectClient(client, _clients))
								it = _clients.begin();
						}
						else if (bytesRead > 0)
							HandleData(client, it);
						else
							++it;
					}
					else
						break;
				}
				PingClients();
				time_t currentTime = time(NULL);
				for (it = _clients.begin(); it != _clients.end();)
				{
					Client* client = it->second;
					if (currentTime - client->GetLastActive() > timeout)
					{
						std::cout << "Client timeout: " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
						DisconnectClient(client, _clients);
					}
					else
						++it;
				}
			}
		}
	}
}

void	Server::HandleData(Client	*client, std::map<int, Client*>::iterator& it)
{
	std::string receivedData = client->GetReceivedData();
	std::istringstream iss(receivedData);
	std::string command;
	int check = 0;
	int count = 0;

	while (iss >> command || (receivedData.empty() == false && receivedData == "\n"))
	{
		if (receivedData.empty() == false && receivedData.at(receivedData.length() - 1) == '\n')
			check = 1;
		if (count == 0 && check == 0)
			client->AddCmd(iss.str());
		receivedData.clear();
		if (check == 1 && count == 0 && client->getCmd().empty() == false)
		{
			std::string entry = iss.str();
			iss.clear();
			iss.str(client->getCmd() + entry);
			iss >> command;
		}
		if (_clients.size() > 0)
		{
			if (client->IsAuthenticated() && !client->GetNickname().empty() && !client->GetUsername().empty() && check == 1)
				HandleCommand(client, command, iss);
			else
			{
				if (check == 1 && !HandleAuthentification(client, command, iss)) 
				{
					it = _clients.begin();
					break ;
				}
			}
		}
		count++;
	}
}

//----------------[GETTERS]---------------------------------------------------

const std::map<int, Client*>& Server::GetClients() const { return _clients; }