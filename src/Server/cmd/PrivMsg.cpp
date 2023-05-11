/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:32 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/11 14:43:45 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
   
void    Server::HandlePrivMsg(Client *client, std::istringstream &iss)
{
	std::string target, message;
	std::istringstream entry(ParsingCmd(iss.str()));
	entry >> target;
	message = entry.str().substr(entry.str().find_first_of(":"), entry.str().length());
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
	message.erase(0, 1);
	client->SendMessage(target, message, true);
	client->UpdateLastActive();
	std::cout << "Client tries to sent message to " << target << ": " << message << std::endl;
	}
}