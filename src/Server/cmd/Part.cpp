/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:40 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/21 18:30:10 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandlePart(Client *client, std::istringstream &iss)
{
    std::string name;
    iss >> name;

	std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (name.empty()) {
        client->SendData("PASS :Not enough parameters\n");
    }
    else if (it == _channels.end()) {
        client->SendData("Channel does not exist\n");
    }
    else if (client->GetChannels().find(name) == client->GetChannels().end()) {
        client->SendData("Client isn't in channel\n");
    }
    else {
        client->GetChannels().erase(name);
        it->second->removeClient(client->GetSocketDescriptor());
    	CloseEmptyChannels();
    }
    if (name.empty() == false)
        while(iss.get() != '\n');
    std::cout << "Client left channel: " << name << std::endl;
}