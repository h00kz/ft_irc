/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:40 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/18 14:42:23 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandlePart(Client *client, std::istringstream &iss)
{
    std::string channel;
    iss >> channel;
    client->LeaveChannel(channel);
    std::cout << "Client leave channel: " << channel << std::endl;
} 