/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:47 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/20 13:38:41 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/*
void	Server::JoinChannel(Client *client, std::string channel)
{
	int channel_pos = this->FindChannel(channel);
	if (channel_pos == -1)
	{
		std::cout << channel_pos << std::endl;
		this->_channels.push_back(createChannel(channel, client));
	}
	else
	{
		std::cout << "came saoule\n";
		this->_channels.at(channel_pos)->addClient(client);
	}
}
*/

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
	if (isValidChannelName(name) == false)
		client->SendData(name += " :Erroneus name name\n");
	else if (this->findChannel(name) == NULL)
	{
		Channel *new_channel = new Channel(name, client);
		if (new_channel == NULL)
			return (client->SendData("name creation failed\n"));
		_channels[name] = new_channel;
		client->enterChannel(name, new_channel);
		std::cout << "Channel " << name << "successfully created" << std::endl;
	}
	/*
	else
	{
		this->JoinChannel(client, channel);
		std::cout << "Client joined channel: " << channel << std::endl;
	}
	*/
}