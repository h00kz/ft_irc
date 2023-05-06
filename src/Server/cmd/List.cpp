/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:45 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/06 18:45:19 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleList(Client *client)
{
    client->SendData("channels available at the moment : \n");
    for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
        client->SendData("\t" + it->second->GetName() + "\n");
}
