/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:29 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/18 14:11:25 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

/* Anciennement dans HandleCommand

		case USER: {
			std::string username, host, server, realname;
			iss >> username >> host >> server;

			std::getline(iss, realname);
			if (!realname.empty() && realname[0] == ':')
				realname.erase(0, 1);
			client->SetUsername(username);
			client->SetHost(host);
			client->SetServer(server);
			client->SetRealname(realname);

			std::cout << "USER called\n";
			std::cout << "Client set username: " << username << std::endl;
			std::cout << "Client set host: " << host << std::endl;
			std::cout << "Client set server: " << server << std::endl;
			std::cout << "Client set real name: " << realname << std::endl;
			break;
*/

void    Server::HandleUser(Client *client, std::istringstream &iss)
{
    if (client->IsAuthenticated())
    {
        std::string username, host, server, realname;
        iss >> username >> host >> server;

        std::getline(iss, realname);
        if (!realname.empty() && realname[0] == ':')
            realname.erase(0, 1);
        client->SetUsername(username);
        client->SetHost(host);
        client->SetServer(server);
        client->SetRealname(realname);
        client->SendData("good username\n");
        std::cout << "USER called\n";
        std::cout << "Client set username: " << username << std::endl;
    }
    else
    {
        std::cout << "Client not authenticated: " << inet_ntoa(client->GetAddress().sin_addr) << " sock: " << client->GetSocketDescriptor() << std::endl;
        if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, client->GetSocketDescriptor(), NULL) == -1)
            std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
        client->Close();
        _clients.erase(client->GetSocketDescriptor());
        return;
    }
}