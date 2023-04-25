/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:45 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/25 10:23:59 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleList(Client *client)
{
    client->SendData("LIST :Channels available at the moment : \n");
    for (std::map<std::string, Channel *>::iterator it; it != _channels.end(); it++)
        client->SendData("\t" + it->second->getName() + "\n");
}
