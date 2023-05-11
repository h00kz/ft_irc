/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:32:14 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/11 14:49:50 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void	Server::HandleKick(Client *client, std::istringstream &iss)
{
    std::string channel, target_name, comment;
	std::istringstream entry(ParsingCmd(iss.str()));

    entry >> channel >> target_name;
    comment = entry.str().substr(entry.str().find_first_of(" "), entry.str().length());
    Channel* chan = FindChannel(channel);
    if (target_name.empty())
        client->SendData("KICK :Need more params\n");
    else if (this->_channels.find(channel) == this->_channels.end())
        client->SendData("KICK :No such channel\n");
    else if (!client->IsInChannel(channel))
        client->SendData("KICK : You are not in channel\n");
    else if (!this->_channels.find(channel)->second->IsOperator(client->GetSocketDescriptor()))
        client->SendData("KICK : You are not operator on this channel\n");
    else if (!this->_channels.find(channel)->second->FindClient(target_name))
        client->SendData("KICK : Target not in channel\n");
    else
    {
        if (client->GetNickname() != target_name)
        {
            this->FindClient(target_name)->LeaveChannel(chan);
            this->_channels.find(channel)->second->RemoveClient(this->FindClient(target_name)->GetSocketDescriptor());
            FindClient(target_name)->SendData("Kicked from " + channel + " " + comment + "\r\n");
        }
        else
        {
            client->SendData("You cannot kick yourself\n");
            std::cout << "client try to kick himself ...\n";
        }
    }
}