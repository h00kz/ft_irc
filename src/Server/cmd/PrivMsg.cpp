/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:32 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/10 16:41:55 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
   
void    Server::HandlePrivMsg(Client *client, std::istringstream &iss)
{
	std::string target, message;
	iss >> target;
	getline(iss, message);
	if (target.empty()) {
		client->SendData("PRIVMSG :No recipient given\n");
	}
	else if (message.empty()) {
		client->SendData("PRIVMSG :No text to send\n");
	}
	else if (message.find(":") == std::string::npos) {
        client->SendData("PRIVMSG :message must be prefixed with \":\"\n");
	}
	else {
	message = message.substr(message.find_first_not_of(" :"), message.length());
	client->SendMessage(target, message, true);
	client->UpdateLastActive();
	std::cout << "Client tries to sent message to " << target << ": " << message << std::endl;
	}
}