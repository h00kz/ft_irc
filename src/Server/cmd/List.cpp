/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:45 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/20 12:11:07 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleList(Client *client)
{
    client->SendData("channels available at the moment : \n");
    for (std::map<std::string, Channel *>::iterator it; it != _channels.end(); it++)
        client->SendData("\t" + it->second->getName() + "\n");
}
