/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:38 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/19 16:28:29 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils.h"
#include "../Server.hpp"

void    Server::HandlePass(Client *client, std::istringstream &iss)
{
    std::string password;
    iss >> password;
    std::cout << "PASS called\n";
    if (client->IsAuthenticated() == true)
        client->SendData("You may not reregister\n");
    else if (getNbWords(password) == 0)
        client->SendData("PASS :Not enough parameters\n");
    else if (password == _serverPasswd && client->IsAuthenticated() == false)
    {
        client->SetAuthenticated(true);
        std::cout << "PASS correct\n";
    }
    else
    {
        std::cout << "Invalid password, client disconnected" << std::endl;
        DisconnectClient(client, _clients);
    }
    return;
}