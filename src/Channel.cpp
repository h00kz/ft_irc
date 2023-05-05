/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:16:29 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 16:18:41 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name, Client *client) : _name(name), _topic(""), _key(""), _invite_only(false), _limit(0), _topic_restricted(false)
{
    t_client *newClient = new t_client;
    newClient->client = client;
    newClient->op = true;
    this->_clients[client->GetSocketDescriptor()] = newClient;
}

Channel::~Channel() {}

void    Channel::AddClient(Client *client)
{
    t_client *newClient = new t_client;

    newClient->client = client;
    newClient->op = false;
    this->_clients.insert(std::pair<int, t_client *>(client->GetSocketDescriptor(), newClient));
}

void    Channel::RemoveClient(int socketDescriptor)
{
    t_client *tmp = _clients.find(socketDescriptor)->second;
    _clients.erase(socketDescriptor);
    delete tmp;
}

Client	*Channel::FindClient(const std::string &name)
{
    for (std::map<int, t_client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        Client* client = it->second->client;
        if (client->GetNickname() == name)
            return (client);
    }
    return (NULL);
}

bool    Channel::IsInvited(int socketDescriptor) const
{
    for (size_t i = 0; i < this->_invited_clients.size(); i++)
    {
        if (this->_invited_clients[i] == socketDescriptor)
            return (true);
    }
    return (false);
}

bool    Channel::IsOperator(int socketDescriptor) const
{
    t_client    *client;

    client = this->_clients.find(socketDescriptor)->second;
    if (client != NULL && client->op)
        return (true); 
    return (false);
}

void    Channel::DeleteInvitation(int socketDescriptor)
{
    size_t i = 0;

    for (; i < _invited_clients.size(); i++)
    {
        if (_invited_clients[i] == socketDescriptor)
            break;
    }
    _invited_clients.erase(_invited_clients.begin() + i);
};

//----------------[SETTERS]---------------------------------------------------

void    Channel::SetTopicRestriction(bool mode) { this->_topic_restricted = mode; }

void    Channel::SetInviteOnly(bool mode) { this->_invite_only = mode; }

void    Channel::SetInvitation(int socketDescriptor)
{
    if (IsInvited(socketDescriptor) == false)
        this->_invited_clients.push_back(socketDescriptor);
};

void    Channel::SetKey(std::string key) { this->_key = key; }

void    Channel::SetTopic(std::string topic) { this->_topic = topic; }

void    Channel::SetOperator(int targetDescriptor, bool mode)
{
    this->_clients.find(targetDescriptor)->second->op = mode;
}

void    Channel::SetLimit(int limit) { this->_limit = limit; }

//----------------[GETTERS]---------------------------------------------------

bool    Channel::IsTopicRestricted(void) const { return (this->_topic_restricted); }

std::string const Channel::GetName(void) const { return (this->_name); }

std::string    Channel::GetTopic(void) const { return (this->_topic); }

std::string Channel::GetKey(void) const { return (this->_key); }

int Channel::GetNbClients(void) const { return (this->_clients.size()); }

bool    Channel::IsInviteOnly(void) const { return (this->_invite_only); }

int Channel::GetLimit(void) const { return (this->_limit); }
