/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:47 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/18 14:31:32 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void	Server::JoinChannel(Client *client, std::string channel)
{
	int channel_pos = this->findChannel(channel);
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

void    Server::HandleJoin(Client *client, std::istringstream &iss)
{
			std::string channel;
			iss >> channel;
			// client->JoinChannel(channel);
			this->JoinChannel(client, channel);
			std::cout << "Client joined channel: " << channel << std::endl;
}
