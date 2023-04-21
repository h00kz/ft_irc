/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:47 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/21 15:21:16 by ffeaugas         ###   ########.fr       */
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
		client->SendData(name += " :Erroneus name name\n");
	}
	else if (it == _channels.end())
	{
		Channel* newChannel = new Channel(name, client);
		_channels.insert(std::make_pair(name, newChannel));
		newChannel->addClient(client);
		client->AddChannel(newChannel);
		std::cout << "Channel " << name << " created and client joined." << std::endl;
	}
	else if (client->IsInChannel(name) == false)
	{
		it->second->addClient(client);
		client->AddChannel(it->second);
		std::cout << "Client " << client->GetNickname() << " joined  channel " << name << "." << std::endl;
		HandleTopic(client, iss);
	}
	else
		std::cout << "Client already in " << name << " channel\n";
	if (name.empty() == false)
    	while(iss.get() != '\n');
}