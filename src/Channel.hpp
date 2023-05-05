/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:15:47 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 16:10:37 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_H
# define CHANNEL_H

# include <iostream>
# include <string> 
# include <map>
# include <vector>
# include "Client.hpp"

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
        bool                                _topic_restricted;
        std::vector<int>                    _invited_clients;
        int                                 _limit;

    public :
        ~Channel();
        Channel(std::string name, Client *client);
        		
        void    AddClient(Client * client);
        void    RemoveClient(int socketDescriptor);
        void    DeleteInvitation(int socketDescriptor);
        void    sendMessage(std::string message);
		
        Client	*FindClient(const std::string &name);
        bool    IsInvited(int socketDescriptor) const;
        bool    IsOperator(int socketDescriptor) const;

        //Getters
        const   std::string GetName(void) const;
        std::string         GetTopic(void) const;
		int                 GetNbClients(void) const;
        std::string         GetKey(void) const;
        int                 GetLimit(void) const;
        bool                IsInviteOnly(void) const;
        bool                IsTopicRestricted(void) const;

        //Setters
        void    SetKey(std::string key);
        void    SetTopicRestriction(bool mode);
        void    SetInviteOnly(bool mode);
        void    SetTopic(std::string topic);
        void    SetInvitation(int socketDescriptor);
        void    SetOperator(int targetDescriptor, bool mode);
        void    SetLimit(int limit);
};

#endif