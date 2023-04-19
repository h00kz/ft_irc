/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:38 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/18 17:39:46 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandlePass(Client *client, std::istringstream &iss)
{
    std::string password;
    iss >> password;
    std::cout << "PASS called\n";
    if (password == _serverPasswd && !client->IsAuthenticated())
    {
        client->SetAuthenticated(true);
        client->SendData("u find the pass\n");
    }
    else if (client->IsAuthenticated())
    {
        client->SendData("Error you may not reregister\n");
        return ;
    }
    // else if (password == "USER" || password == "")
    // {
    //     client->SendData("Error PASS not enough parameters\n");
    //     if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
    //         std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
    //     client->Close();
    //     _clients.erase(client->GetSocketDescriptor());
    //     return ;
    // }
    else
    {
        DisconnectClient(client, _clients);
        return;
    }
}