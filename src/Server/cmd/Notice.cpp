/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:32 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/10 16:48:32 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
   
void    Server::HandleNotice(Client *client, std::istringstream &iss)
{
	std::string target, message;
	iss >> target;
	getline(iss, message);
	if (target.empty() || message.empty() || message.find(":") == std::string::npos) {
		return ;
	}
	message = message.substr(message.find_first_not_of(" :"), message.length());
	client->SendMessage(target, message, false);
	client->UpdateLastActive();
	std::cout << "Client tries to notice message to " << target << ": " << message << std::endl;
}