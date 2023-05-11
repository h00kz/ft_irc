/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:29 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/11 17:13:47 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleUser(Client *client, std::istringstream &iss)
{
    std::string username, host, server, realname;
	std::istringstream entry(ParsingCmd(iss.str()));
	
	entry >> username >> host >> server;
    if (entry.str().find_first_of(":") == std::string::npos)
        std::cout << "tamer le pelican\n";
    if (entry.str().find_first_of(":") != std::string::npos)
	    realname = entry.str().substr(entry.str().find_first_of(":"), entry.str().length());
    if (realname.empty() == true)
        client->SendData("USER :Not enough parameters\n");
    else if (realname.at(0) != ':' || realname.find_last_of(":") != 0)
    {
        std::cout << realname << std::endl;
        client->SendData("USER :realname must be prefixed with \":\"\n");
    }
    else if (client->GetUsername().empty() == false)
        client->SendData("USER :You may not reregister\n");
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