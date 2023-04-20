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
/*
    t_client *new_client = new t_client;

    new_client->client = client;
    int client_pos = this->findClient(client);
    if (client_pos == -1)
        _clients[client->GetNickname] = new_client;
        this->_clients.push_back(new_client);
    else
        std::cout << "Client is already in this channel" << std::endl;
*/
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