#include "../Server.hpp"

void	Server::HandleKick(Client *client, std::istringstream &iss)
{
    std::string channel, target_name;
    iss >> channel >> target_name;
    if (target_name.empty())
        client->SendData("KICK :Need more params\n");
    else if (this->_channels.find(channel) == this->_channels.end())
        client->SendData("KICK :No such channel\n");
    else if (!client->IsInChannel(channel))
        client->SendData("KICK : You are not in channel\n");
    else if (!this->_channels.find(channel)->second->IsOperator(client->GetSocketDescriptor()))
        client->SendData("KICK : You are not operator on this channel\n");
    else if (!this->_channels.find(channel)->second->findClient(target_name))
        client->SendData("KICK : Target not in channel\n");
    else
    {
        this->findClient(target_name)->GetChannels().erase(channel);
        this->_channels.find(channel)->second->removeClient(this->findClient(target_name)->GetSocketDescriptor());
    }
    if (target_name.empty() == false)
        while(iss.get() != '\n');
}