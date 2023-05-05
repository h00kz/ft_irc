/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:29 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 11:31:39 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleUser(Client *client, std::istringstream &iss)
{
    std::string username, host, server, realname;

    std::cout << "USER called\n";
    iss >> username >> host >> server >> realname;
    if (realname.empty() == true)
        client->SendData("USER :Not enough parameters\n");
    else if (realname[0] != ':')
    {
        std::cout << realname << std::endl;
        client->SendData("USER :realname must be prefixed with \":\"\n");
    }
    else if (client->GetUsername().empty() == false)
        client->SendData("You may not reregister\n");
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
	if (realname.empty() == false)
    	while(iss.get() != '\n');
}