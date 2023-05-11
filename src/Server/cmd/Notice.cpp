/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:32 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/11 14:44:56 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
   
void    Server::HandleNotice(Client *client, std::istringstream &iss)
{
	std::string target, message;
	std::istringstream entry(ParsingCmd(iss.str()));
	entry >> target;
	message = entry.str().substr(entry.str().find_first_of(":"), entry.str().length());
	if (target.empty() || message.empty() || message.find(":") == std::string::npos) {
		return ;
	}
	message.erase(0 ,1);
	client->SendMessage(target, message, false);
	client->UpdateLastActive();
	std::cout << "Client tries to notice message to " << target << ": " << message << std::endl;
}