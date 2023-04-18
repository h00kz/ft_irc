/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:42 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/04/18 14:42:22 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleNick(Client *client, std::istringstream &iss)
{
    std::string nickname;
    iss >> nickname;
    client->SetNickname(nickname);
    std::cout << "NICK called\n";
    std::cout << "Client set nickname: " << nickname << std::endl;
}			