/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:35 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 11:25:01 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandlePong(Client *client)
{
    client->UpdateLastActive();
    std::cout << "PING recieve from " << inet_ntoa(client->GetAddress().sin_addr) << ":"
    << ntohs(client->GetAddress().sin_port) << std::endl;
}