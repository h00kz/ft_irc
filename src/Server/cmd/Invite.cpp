/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:32:04 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/15 16:08:44 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleInvite(Client *client, std::istringstream &iss)
{
    std::string nickname, channelName;
    Client  *target;
    Channel *channel;

    iss >> nickname >> channelName;
    target = FindClient(nickname);
    channel = FindChannel(channelName);
    if (channelName.empty()) {
        client->SendData("INVITE :Need more params\n");
    }
    else if (target == NULL) {
        client->SendData("INVITE :No such nick\n");
    }
    else if (channel == NULL) { 
        std::cout << "Channel to invite in does not exists\n";
    }
    else if (target->GetChannels().find(channelName) != target->GetChannels().end()) {
        client->SendData("INVITE :Target already on channel\n");
    }
    else if (client->GetChannels().find(channelName) == client->GetChannels().end()) {
        client->SendData("INVITE :Client not is channel\n");
    }
    else if (channel->IsOperator(client->GetSocketDescriptor()) == false) {
        client->SendData("INVITE :Client not operator on this channel\n");
    }
    else {
        channel->SetInvitation(target->GetSocketDescriptor());
        std::cout << nickname << " successfully invited on " << channel->GetName() << "\n";
    }
    if (channelName.empty() == false)
        while (iss.get() != '\n');
}