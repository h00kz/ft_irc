#include "Channel.hpp"


void    Channel::sendMessage(std::string message)
{
    for (std::vector<t_client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        (*it)->client->SendData(message);
    }

}  //Se balade dans le tablo de client et chaque fois que client[i].online = true, lui balance le message

Channel::Channel(std::string name, Client *client)
{
    this->_name = name;
//    this->_hostClient->client = client;
    this->addClient(client);
}

int     Channel::findClient(Client *client)
{
    int i = 0;
	for (std::vector<t_client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if ((*it)->client == client)
            return (i);
        i++;
    }
    return (-1);
}

Channel::~Channel()
{
    for (std::vector<t_client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (*it != NULL)
            delete (*it);
    }
}

void    Channel::setMode(std::string new_mode)
{

}

void    Channel::addClient(Client *client)
{
    t_client *new_client = new t_client;

    new_client->client = client;
    int client_pos = this->findClient(client);
    if (client_pos == -1)
        this->_clients.push_back(new_client);
    else
        std::cout << "Client is already in this channel" << std::endl;
}

std::string Channel::getName(void)
{
    return (this->_name);
}