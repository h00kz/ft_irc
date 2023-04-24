/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:38 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/21 15:21:48 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils.h"
#include "../Server.hpp"

void    Server::HandlePass(Client *client, std::istringstream &iss)
{
    std::string password;

    getline(iss, password);
    if (!password.empty() && password.find_first_not_of(" ") != std::string::npos && password.find_last_not_of("\r\n") != std::string::npos)
        password = password.substr(password.find_first_not_of(" "), password.find_last_not_of("\r\n"));
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
        return;
    }
    else
    {
        std::cout << "|" << password << "|"  << std::endl;
        std::cout << "Invalid password, client disconnected" << std::endl;
        DisconnectClient(client, _clients);
    }
    // if (password.empty() == false)
    //     while(iss.get() != '\n');
}