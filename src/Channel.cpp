#include "Channel.hpp"

Channel::Channel(std::string name, Client *client) : _name(name), _topic("")
{
    t_client *newClient = new t_client;
    newClient->client = client;
    newClient->op = true;
    this->_clients[client->GetSocketDescriptor()] = newClient;
}

Channel::~Channel()
{
}

/*
void    Channel::setMode(std::string new_mode)
{
}
*/

void    Channel::addClient(Client *client)
{
    t_client *newClient = new t_client;

    newClient->client = client;
	printf("CLIENT : %p\n", findClient(newClient->client->GetNickname()));
	if (findClient(newClient->client->GetNickname()) != NULL)
	{
    	_clients.insert(std::make_pair(newClient->client->GetSocketDescriptor(), newClient));
		for (std::map<int, t_client*>::iterator it = _clients.begin();it != _clients.end();++it)
			std::cout << it->second->client->GetNickname() << std::endl;
	}
    else
        std::cout << "Client is already in this channel" << std::endl;
}

std::string const Channel::getName(void) const
{
    return (this->_name);
}

Client	*Channel::findClient(const std::string &name)
{
    for (std::map<int, t_client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        Client* client = it->second->client;
        if (client->GetNickname() == name)
            return (client);
    }
    return (NULL);
}