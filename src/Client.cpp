/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:18:21 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 11:46:04 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server/Server.hpp"

Client::Client(int socketDescriptor, struct sockaddr_in address, Server* server)
	: _socketDescriptor(socketDescriptor), _address(address)
{
	UpdateLastActive();
	_server = server;
	_authenticated = false;
	_username = "";
	_nickname = "";
}

Client::~Client()
{
	Close();
}

Channel	*Client::FindChannel(const std::string &name)
{
    
    std::map<std::string, Channel *>::iterator it = _channels.find(name);
    if (it != _channels.end())
        return (it->second);
    return (NULL);
}

void Client::AddChannel(Channel* channel)
{
	_channels.insert(std::make_pair(channel->GetName(), channel));
}

void	Client::LeaveChannels(void)
{
	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it)
	{
		it->second->RemoveClient(_socketDescriptor);
	}
	_channels.clear();
}

void Client::Close()
{
	if (_socketDescriptor != -1)
		close(_socketDescriptor);
}

void Client::SendMessage(const std::string& target, const std::string& message)
{
	if (_server)
	{
		std::string formattedMessage = ":" + _nickname + " PRIVMSG " + target + " :" + message + "\r\n";
		if (target[0] == '#')
			_server->BroadcastMessage(target, formattedMessage, this);
		else if (target[0] != '#')
			_server->SendPrivateMessage(target, formattedMessage, this);
		else
			std::cout << "This channel not exist.\n";
	}
}

void Client::SendData(const std::string& data)
{
	if (_socketDescriptor != -1)
		send(_socketDescriptor, data.c_str(), data.length(), 0);
}

int Client::ReceiveData()
{
	char buffer[1024];
	int bytesRead = recv(_socketDescriptor, buffer, sizeof(buffer) - 1, 0);

	if (bytesRead > 0)
	{
		buffer[bytesRead] = '\0';
		_receivedData = std::string(buffer);
	}

	return bytesRead;
}

void Client::UpdateLastActive()
{
	_lastActive = time(NULL);
}

//----------------[SETTERS]---------------------------------------------------

void Client::SetAuthenticated(bool b) { _authenticated = b; }

void Client::SetNickname(const std::string &nickname) { _nickname = nickname; }

void Client::SetRealname(const std::string &realname) { _realName = realname; }

void Client::SetHost(const std::string &host) { _hostName = host; }

void Client::SetServer(const std::string &servername) { _serverName = servername; }

void Client::SetUsername(std::string const &username) { _username = username; }

//----------------[GETTERS]---------------------------------------------------

std::map<std::string, Channel*>& Client::GetChannels() { return _channels; }

time_t Client::GetLastActive() const { return _lastActive; }

const std::string &Client::GetReceivedData() const { return _receivedData; }

int Client::GetSocketDescriptor() const { return _socketDescriptor; }

const struct sockaddr_in &Client::GetAddress() const { return _address; }

std::string const &Client::GetNickname() const { return _nickname; }

std::string const &Client::GetRealname() const { return _realName; }

std::string const &Client::GetHost() const { return _hostName; }

std::string const &Client::GetServer() const { return _serverName; }

std::string const &Client::GetUsername() const { return _username; }

bool Client::IsInChannel(const std::string &channel) const
{
	return _channels.find(channel) != _channels.end();
}

bool Client::IsConnected() const
{
	int error = 0;
	socklen_t len = sizeof(error);
	int retval = getsockopt(_socketDescriptor, SOL_SOCKET, SO_ERROR, &error, &len);
	if (retval != 0)
		return false;
	if (error != 0)
		return false;
	return true;
}

bool Client::IsAuthenticated() const { return (_authenticated); }