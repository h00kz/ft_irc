/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:42 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 15:27:43 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

bool    Server::isAvailableNickname(std::string nickname)
{
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
    {
        if (it->second->GetNickname().compare(nickname) == 0)
            return (false);
    }
    return (true);
}

static bool    isValidNickname(std::string nickname)
{
	if (nickname.size() > 9)
		return (false);
	if (nickname.find_first_of(" \t\r\n\f\v") != std::string::npos)
		return (false);
	return (true);
}

void    Server::HandleNick(Client *client, std::istringstream &iss)
{
    std::string nickname;

    iss >> nickname;
    std::cout << "NICK called\n";
	if (nickname.empty()) {
        client->SendData("NICK :Not enough parameters\n");
		return ;
	}
	if (isValidNickname(nickname) == false) {
        client->SendData(nickname += " :Erroneus nickname\n");
	}
	else if (isAvailableNickname(nickname) == false) {
        client->SendData(nickname += " :Nickname is already owned by another client\n");
	}
	else {
		if (client->GetNickname() == "")
    		std::cout << "Client set Nickname : " << nickname << std::endl;
	    else
			std::cout << client->GetNickname() << " changed his nickname to " << nickname << std::endl;
		client->SetNickname(nickname);
	}
	if (nickname.empty() == false)
    	while(iss.get() != '\n');
}			