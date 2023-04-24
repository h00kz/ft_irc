/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: h00kz <h00kz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:32 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/22 22:24:44 by h00kz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
   
void    Server::HandlePrivMsg(Client *client, std::istringstream &iss)
{
	std::string target, message;
	iss >> target;
	getline(iss, message);
	if (message.find(":") == std::string::npos)
	{
		client->SendData("Usage : PRIVMSG [channel] :[message]\n");
		return ;
	}
	message = message.substr(message.find_first_not_of(" :"), message.length());
	client->SendMessage(target, message);
	client->UpdateLastActive();
	std::cout << "Client try to sent message to " << target << ": " << message << std::endl;
}