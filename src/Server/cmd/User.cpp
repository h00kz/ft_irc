/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:29 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/19 16:37:25 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleUser(Client *client, std::istringstream &iss)
{
    if (client->IsAuthenticated() == true)
    {
        std::string username, host, server, realname;

    
        iss >> username >> host >> server;
        std::cout << "USER called\n";
        std::getline(iss, realname);
        if (realname.empty() == true)
            client->SendData("USER :Not enough parameters");
        else if (realname[0] != ':')
            client->SendData("USER :realname must be prefixed with \":\"\n");
        else
        {
            realname.erase(0, 1);
            client->SetUsername(username);
            client->SetHost(host);
            client->SetServer(server);
            client->SetRealname(realname);
            std::cout << "Client set username: " << username << std::endl;
            std::cout << "Client set host: " << host << std::endl;
            std::cout << "Client set server: " << server << std::endl;
            std::cout << "Client set real name: " << realname << std::endl;
        }
    }
}