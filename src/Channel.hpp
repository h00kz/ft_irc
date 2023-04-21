#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <string> 
#include <map>
#include "Client.hpp"

typedef struct s_client
{
    Client  *client;
    bool    op;

}   t_client;

class Channel
{
    private :
        std::map<int, t_client *>           _clients;
        std::string                         _name;
        std::string                         _topic;
        std::string                         _key;

    public :
        ~Channel();
        Channel(std::string name, Client *client);
        
		
        void    addClient(Client * client);
        void    removeClient(int socketDescriptor);
		Client	*findClient(const std::string &name);
        
        void    sendMessage(std::string message);
        
        //Getters
        const   std::string getName(void) const;
        std::string         getTopic(void) const;
		int getNbClients(void) const;

        //Setters
        void    setKey(std::string key);
        void    setTopic(std::string topic);
        // void    setMode(std::string);
};

#endif