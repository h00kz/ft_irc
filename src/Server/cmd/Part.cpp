/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:40 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/11 16:52:46 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandlePart(Client *client, std::istringstream &iss)
{
    std::string name;

    if (iss.str().empty() == false)
        name = ParsingCmd(iss.str());
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (name.empty()) {
        client->SendData("PART :Not enough parameters\n");
    }
    else if (it == _channels.end()) {
        client->SendData("PART :Channel does not exist\n");
    }
    else if (client->GetChannels().find(name) == client->GetChannels().end()) {
        client->SendData("PART :Client isn't in channel\n");
    }
    else {
        client->GetChannels().erase(name);
        it->second->RemoveClient(client->GetSocketDescriptor());
    	CloseEmptyChannels();
        std::cout << "Client left channel: " << name << std::endl;
    }
}