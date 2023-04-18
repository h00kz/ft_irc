#ifndef CHANNEL_H
#define CHANNEL_H

#include "Client.hpp"
#include <iostream>
#include <string> 

#define operatir

typedef struct s_client
{
    Client  *client;
    bool    op;
    bool    online;
    bool    ;
    bool    ;
    bool    ;
    bool    ;
    bool    ;

}   t_client;
/*
o - donne/retire les privilèges d'opérateur de canal
p - drapeau de canal privé
s - drapeau de canal secret
i - drapeau de canal accessible uniquement sur invitation
t - drapeau de sujet de canal modifiable uniquement par les opérateurs
n - pas de messages dans un canal provenant de clients à l'extérieur du canal
m - canal modéré
l - définit le nombre maximal de personnes dans un canal
b - définit un masque de bannissement pour interdire l'accès à des utilisateurs
v - donne/retire la possibilité de parler dans un canal modéré
k - définit la clé du canal (mot de passe)
*/


class Channel
{
    private :
        t_client                    *_hostClient;
        std::vector<t_client *>     _clients;
        std::string                 _name;
        std::string                 _topic;
        // status     
        

    public :
        ~Channel();
        Channel(std::string name, Client *client);
        void    setMode(std::string);
		void    addClient(Client * client);
        int     findClient(Client *client);
        void    sendMessage(std::string message);  //Se balade dans le tablo de client et chaque fois que client[i].online = true, lui balance le message
        std::string getName(void);
};

#endif