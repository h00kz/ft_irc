/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:37 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/20 15:26:41 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::PingClients()
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		Client* client = it->second;
		if (client->IsConnected())
		{
			time_t currentTime = time(NULL);
			if (currentTime - client->GetLastActive() > 60)
			{
				std::string pingCommand = "PING\r\n";
				client->SendData(pingCommand);
				client->UpdateLastActive();
			}
		}
	}
}

void    Server::HandlePing(Client *client)
{
    client->UpdateLastActive();
    std::string response = "PONG\r\n";
    client->SendData(response);
    std::cout << "PONG sent to " << inet_ntoa(client->GetAddress().sin_addr) << ":" << ntohs(client->GetAddress().sin_port) << std::endl;
}