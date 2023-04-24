#include "../Server.hpp"

void	Server::HandleKick(Client *client, std::istringstream &iss)
{
    std::string channel, target_name, comment;
    iss >> channel >> target_name;
    getline(iss, comment);
    Channel* chan = findChannel(channel);
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
        if (client->GetNickname() != target_name)
        {
            this->findClient(target_name)->LeaveChannel(chan);
            this->_channels.find(channel)->second->removeClient(this->findClient(target_name)->GetSocketDescriptor());
            findClient(target_name)->SendData("KICK from " + channel + " " + comment + "\r\n");
        }
    }
}