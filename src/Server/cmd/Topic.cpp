/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:12:42 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 16:24:13 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void    Server::HandleTopic(Client *client, std::istringstream &iss)
{
    std::string channelName, topicName;

    iss >> channelName;
    iss >> topicName;
	std::cout << "TOPIC called\n";
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (channelName.empty()) {
        client->SendData("TOPIC :Need more params\n");
        return ;
    }
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
        client->SendData(channelName.append(" no topic is set\n"));
    }
    else if (topicName.empty() == true) {
        client->SendData(channelName.append(" topic : ").append(it->second->GetTopic().append("\n")));
    }
    else if (topicName[0] != ':') {
        client->SendData("TOPIC :topic must be prefixed with \":\"\n");
    }
    else if (topicName[1] == '\0') {
        client->SendData("TOPIC :topic must not be empty\n");
    }
    else {
        topicName.erase(0, 1);
        it->second->SetTopic(topicName);
        while (iss >> topicName)
        {
            it->second->SetTopic(it->second->GetTopic().append(" "));
            it->second->SetTopic(it->second->GetTopic().append(topicName));    
        }
        std::cout << channelName << " topic set\n";
        return ;
    }
    if (topicName.empty() == false)
        while(iss.get() != '\n');
}