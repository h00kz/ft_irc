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

    public :
        ~Channel();
        Channel(std::string name, Client *client);
        // void    setMode(std::string);
		void    addClient(Client * client);
        void    sendMessage(std::string message);
		Client	*findClient(const std::string &name);
        
        //Getters
        const   std::string getName(void) const;
};

#endif