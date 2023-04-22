/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:47 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/22 16:34:50 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

static bool    isValidChannelName(std::string channel)
{
	if (channel.size() > 200)
		return (false);
	if (channel.empty() == false && channel[0] != '#' && channel[0] != '&')
		return (false);
	if (channel.find_first_of(", \t\r\n\f\v") != std::string::npos)
		return (false);
	return (true);
}

void    Server::HandleJoin(Client *client, std::istringstream &iss)
{
	std::string name;
	iss >> name;

	std::cout << "JOIN called\n";
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (name.empty()) {
        client->SendData("JOIN :Need more params\n");
	}
	else if (isValidChannelName(name) == false) {
		client->SendData(name += " :Erroneus channel name\n");
	}
	else if (client->IsInChannel(name)) {
		client->SendData("JOIN :Already in channel\n");
	}
	else if (client->GetChannels().size() == 10) {
		client->SendData("JOIN :Can't join more than 10 channels\n");
	}
	else if (it == _channels.end()) {
		Channel* newChannel = new Channel(name, client);
		_channels.insert(std::make_pair(name, newChannel));
		client->AddChannel(newChannel);
		std::cout << "Channel " << name << " created and client joined." << std::endl;
	}
	else if (it->second->IsInviteOnly() && it->second->IsInvited(client->GetSocketDescriptor() == false)) {
		client->SendData(name += " :invitation required to join this channel\n");
	}
	else {
		it->second->addClient(client);
		client->AddChannel(it->second);
		std::cout << "Client " << client->GetNickname() << " joined  channel " << name << "." << std::endl;
		if (it->second->IsInviteOnly())
			it->second->DeleteInvitation(client->GetSocketDescriptor());
		// HandleTopic(client, iss);
	}
	if (name.empty() == false)
    	while(iss.get() != '\n');
}