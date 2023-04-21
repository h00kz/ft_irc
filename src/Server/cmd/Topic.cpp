

#include "../Server.hpp"
#include <string>

void    Server::HandleTopic(Client *client, std::istringstream &iss)
{
    std::string channelName, topicName;
    iss >> channelName;

	std::cout << "TOPIC called\n";
	std::map<std::string, Channel*>::iterator it = _channels.find(channelName);
    if (channelName.empty()) {
        client->SendData("TOPIC :Need more params\n");
        return ;
    }
    iss >> topicName;
    if (it == _channels.end() || (channelName[0] != '#' && channelName[0] != '&')) {
        client->SendData("Invalid channel\n");
    }
    else if (client->IsInChannel(channelName) == false) {
        client->SendData("Topic :Can't change topic outside of channel\n");
    }
    else if (topicName.empty() == true && it->second->getTopic().empty() == true) {
        client->SendData(channelName.append(" as not topic yet\n"));
    }
    else if (topicName.empty() == true) {
        client->SendData(channelName.append(" topic : ").append(it->second->getTopic().append("\n")));
    }
    else if (topicName[0] != ':') {
        client->SendData("Topic :topic must be prefixed with \":\"\n");
    }
    else {
        topicName.erase(0, 1);
        it->second->setTopic(topicName);
        while (iss >> topicName)
        {
            it->second->setTopic(it->second->getTopic().append(" "));
            it->second->setTopic(it->second->getTopic().append(topicName));    
        }
        std::cout << channelName << " topic set\n";
        return ;
    }
    if (topicName.empty() == false)
        while(iss.get() != '\n');
}