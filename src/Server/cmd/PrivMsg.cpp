/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivMsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:32 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/21 09:15:53 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"
   
void    Server::HandlePrivMsg(Client *client, std::istringstream &iss)
{
    std::string target, message;
    iss >> target;
    getline(iss, message);
    if (!message.empty() && message[0] == ':')
        message.erase(0, 1);
    client->SendMessage(target, message);
    std::cout << "Client sent message to " << target << ": " << message << std::endl;
}