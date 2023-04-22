#include "../Server.hpp"

void	Server::HandleKick(Client *client, std::istringstream &iss)
{
    std::string channel, name;
    iss >> channel >> name;
    if (name.empty())
        client->SendData("KICK :Need more params\n");
    else if (this->_channels.find(channel) == this->_channels.end())
        client->SendData("KICK :No such channel\n");
    else if ()


}