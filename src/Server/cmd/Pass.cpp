/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:38 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 11:00:45 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandlePass(Client *client, std::istringstream &iss)
{
    std::string password;

    iss >> password;
    std::cout << "PASS called\n";
    if (password.empty()) {
        client->SendData("PASS :Not enough parameters\n");
    }
    else if (client->IsAuthenticated() == true)
        client->SendData("You may not reregister\n");
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
    if (password.empty() == false)
        while(iss.get() != '\n');
    return;
}