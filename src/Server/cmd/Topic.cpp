/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:35:57 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/11 15:00:16 by jlarrieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleTopic(Client *client, std::istringstream &iss)
{
    std::string channelName, topicName;
	std::istringstream entry(ParsingCmd(iss.str()));

	entry >> channelName;
	std::cout << "TOPIC called\n";
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (channelName.empty()) {
        client->SendData("TOPIC :Need more params\n");
        return ;
    }
    entry >> topicName;
    if (it == _channels.end() || (channelName[0] != '#' && channelName[0] != '&')) {
        client->SendData("Invalid channel\n");
    }
    else if (client->IsInChannel(channelName) == false) {
        client->SendData("TOPIC :Can't change topic outside of channel\n");
    }
    else if (it->second->IsTopicRestricted() && it->second->IsOperator(client->GetSocketDescriptor()) == false) {
        client->SendData("TOPIC :You must be operator to see or change topic\n");
    }
    else if (topicName.empty() == true && it->second->GetTopic().empty() == true) {
        client->SendData(channelName.append(" as not topic yet\n"));
    }
    else if (topicName.empty() == true) {
        client->SendData(channelName.append(" topic : ").append(it->second->GetTopic().append("\n")));
    }
    else if (topicName[0] != ':') {
        client->SendData("TOPIC :topic must be prefixed with \":\"\n");
    }
    else {
        topicName.erase(0, 1);
        it->second->setTopic(topicName);
        while (entry >> topicName)
        {
            it->second->setTopic(it->second->GetTopic().append(" "));
            it->second->setTopic(it->second->GetTopic().append(topicName));    
        }
        std::cout << channelName << " topic set\n";
        return ;
    }
}
