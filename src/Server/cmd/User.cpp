/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:29 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/25 10:15:26 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleUser(Client *client, std::istringstream &iss)
{
    std::string username, host, server, realname;

    std::cout << "USER called\n";
    iss >> username >> host >> server;
    getline(iss, realname);
    realname = realname.substr(0, realname.find_last_not_of("\r\n") + 1);
    if (realname.empty() == true)
        client->SendData("USER :Not enough parameters\n");
    else if (realname[1] != ':')
    {
        std::cout << realname << std::endl;
        client->SendData("USER :realname must be prefixed with \":\"\n");
    }
    else if (client->GetUsername().empty() == false)
        client->SendData("USER :You may not reregister\n");
    else
    {
        realname.erase(0, 2);
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