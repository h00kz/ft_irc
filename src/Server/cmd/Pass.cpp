/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:38 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/11 17:28:50 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandlePass(Client *client, std::istringstream &iss)
{
    std::string password;

    if (iss.str().empty() == false)
		password = ParsingCmd(iss.str());
    if (password.empty()) {
        client->SendData("PASS :Not enough parameters\n");
    }
    else if (client->IsAuthenticated() == true)
        client->SendData("PASS :You may not reregister\n");
    else if (password == _serverPasswd && client->IsAuthenticated() == false)
    {
        client->SetAuthenticated(true);
        std::cout << "PASS correct\n";
        return;
    }
    else
    {
        std::cout << "Invalid password" << std::endl;
        client->SendData("PASS :Invalid password\n");
    }
}