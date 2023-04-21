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

Channel	*Client::findChannel(const std::string &name)
{
    
    std::map<std::string, Channel *>::iterator it = _channels.find(name);
    if (it != _channels.end())
        return (it->second);
    return (NULL);
}

void Client::UpdateLastActive()
{
	_lastActive = time(NULL);
}

time_t Client::GetLastActive() const
{
	return _lastActive;
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

void	Client::enterChannel(const std::string& name, Channel *channel)
{
	_channels[name] = channel;
	for (std::map<std::string, Channel*>::iterator it = _channels.begin();it != _channels.end();++it)
		std::cout << it->first.c_str() << std::endl;
}

bool Client::IsInChannel(const std::string &channel) const
{
	return _channels.find(channel) != _channels.end();
}

/*
void Client::JoinChannel(const std::string &channel)
{
	if (this->IsInChannel(channel))
		return;
	_channels.push_back(channel);
    if (_server)
        _server->AddChannel(channel);
}
*/

void Client::LeaveChannel(const std::string &channel)
{
	
}


const std::map<std::string, Channel*>& Client::GetChannels() const
{
	return _channels;
}

void Client::AddChannel(Channel* channel)
{
	_channels.insert(std::make_pair(channel->getName(), channel));
}

const std::string &Client::GetReceivedData() const
{
	return _receivedData;
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

int Client::GetSocketDescriptor() const
{
	return _socketDescriptor;
}

void Client::Close()
{
	if (_socketDescriptor != -1)
		close(_socketDescriptor);
}

void Client::SendData(const std::string& data)
{
	if (_socketDescriptor != -1)
		send(_socketDescriptor, data.c_str(), data.length(), 0);
}

//----------------SETTERS & GETTERS---------------------------------------------------

void Client::SetAuthenticated(bool b)
{
	_authenticated = b;
}

bool Client::IsAuthenticated()
{
	return (_authenticated);
}

const struct sockaddr_in &Client::GetAddress() const
{
	return _address;
}

void Client::SetNickname(const std::string &nickname)
{
	_nickname = nickname;
}

std::string const &Client::GetNickname() const
{
	return _nickname;
}

void Client::SetRealname(const std::string &realname)
{
	_realName = realname;
}

std::string const &Client::GetRealname() const
{
	return _realName;
}

void Client::SetHost(const std::string &host)
{
	_hostName = host;
}

std::string const &Client::GetHost() const
{
	return _hostName;
}

void Client::SetServer(const std::string &servername)
{
	_serverName = servername;
}

std::string const &Client::GetServer() const
{
	return _serverName;
}


void Client::SetUsername(std::string const &username)
{
	_username = username;
}

std::string const &Client::GetUsername() const
{
	return _username;
}