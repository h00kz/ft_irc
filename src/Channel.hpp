#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <string> 
#include <map>
#include <vector>
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
        bool                                _invite_only;
        bool                                _topic_allowed;
        std::vector<int>                    _invited_clients;
        int                                 _limit;

    public :
        ~Channel();
        Channel(std::string name, Client *client);
        
		
        void    addClient(Client * client);
        void    removeClient(int socketDescriptor);
		Client	*findClient(const std::string &name);

        void    DeleteInvitation(int socketDescriptor);

        bool    IsInvited(int socketDescriptor) const;
        bool    IsOperator(int socketDescriptor) const;
        
        void    sendMessage(std::string message);
        
        //Getters
        const   std::string getName(void) const;
        std::string         getTopic(void) const;
		int                 getNbClients(void) const;
        std::string         getKey(void) const;
        int                 getLimit(void) const;
        bool                IsInviteOnly(void) const;
        bool                IsTopicRestricted(void) const;

        //Setters
        void    SetKey(std::string key);
        void    SetTopicRestriction(bool mode);
        void    SetInviteOnly(bool mode);
        void    setTopic(std::string topic);
        void    SetInvitation(int socketDescriptor);
        void    SetOperator(int targetDescriptor, bool mode);
        void    SetLimit(int limit);
        // void    setMode(std::string);
};

#endif