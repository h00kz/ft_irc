/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 13:31:44 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/15 15:52:07 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Server.hpp"

void Server::HandleInviteMode(Client *client, Channel *channel, char operation)
{
    if (operation == '+' && channel->IsInviteOnly() == false) {
        channel->SetInviteOnly(true);
        client->SendData("Invite only mode : true\n");
    }
    else if (operation == '-' && channel->IsInviteOnly()) {
        channel->SetInviteOnly(false);
        client->SendData("Invite only mode : false\n");
    }
}

void Server::HandleTopicMode(Client *client, Channel *channel, char operation)
{
    if (operation == '+' && channel->IsTopicRestricted() == false) {
        channel->SetTopicRestriction(true);
        client->SendData("Topic restricted mode : true\n");
    }
    else if (operation == '-' && channel->IsTopicRestricted() == true) {
        channel->SetTopicRestriction(false);
        client->SendData("Topic restricted mode : false\n");
    }
}

void Server::HandleKeyMode(Client *client, Channel *channel, std::istringstream &iss, char operation)
{
    std::string key;
    
    if (operation == '+')
    {
        iss >> key;
        if (key.empty())
            client->SendData("MODE :Key is missing\n");
        else
            channel->SetKey(key);
    }
    else
        channel->SetKey("");
}

void Server::HandleOperatorMode(Client *client, Channel *channel, std::istringstream &iss, char operation)
{
    std::string targetName;
    Client  *target;

    iss >> targetName;
    target = channel->FindClient(targetName);
    if (target == NULL)
    {
        client->SendData("MODE :Operator target is missing\n");
        return ;
    }
    else if (targetName.empty()) {
        client->SendData("MODE :Operator target is missing\n");
    }
    else if (target->IsInChannel(channel->GetName()) == false) {   
        client->SendData("MODE :Operator target " + target->GetNickname() + " is not in channel\n");
    }
    else if (operation == '+') {
        channel->SetOperator(target->GetSocketDescriptor(), true);
        client->SendData("MODE :New operator set\n");
    }
    else {
        channel->SetOperator(target->GetSocketDescriptor(), false);
        client->SendData("MODE :Operator removed\n");
    }
}

void Server::HandleLimitMode(Client *client, Channel *channel, std::istringstream &iss, char operation)
{
    std::string limit;
    int limitNb;

    if (operation == '+')
    {
        iss >> limit;
        if (limit.empty())
            client->SendData("MODE :Limit number is missing\n");
        else
        {
            limitNb = std::atoi(limit.c_str());
            if (limitNb < 1 || limitNb > 1000)
                client->SendData("MODE :Invalid limit [Allowed range : 1 - 1000]\n");
            else if (limitNb < channel->GetNbClients())
                client->SendData("MODE :Limit cannot be inferior than number of users in channel\n");
            else
            {
                channel->SetLimit(limitNb);
                client->SendData("MODE :Limit has been set\n");
            }
        }
    }
    else
    {
        channel->SetLimit(0);
        client->SendData("MODE :Limit has been removed\n");
    }
}

void    Server::HandleMode(Client *client, std::istringstream &iss)
{

    std::string channelName, modes, trash;
    iss >> channelName >> modes;

    Channel *channel = FindChannel(channelName);
    if (modes.empty()) {
        client->SendData("MODE :Need more params\n");
    }
    else if (channel == NULL) {
        client->SendData("MODE :No such channel\n");
    }
    else if (modes[0] != '-' && modes[0] != '+') {
        client->SendData("MODE :'+' or '-' required before mode char\n");
    }
    else if (client->IsInChannel(channelName) == false) {   
        client->SendData("MODE :You are not in channel\n");
    }
    else if (channel->IsOperator(client->GetSocketDescriptor()) == false) {
        client->SendData("MODE :You are not operator on this channel\n");
    }
    else
    {
        for (size_t i = 1; i < modes.length(); i++)
        {
            if (modes[i] == 'i')
                HandleInviteMode(client, channel, modes[0]);
            else if (modes[i] == 't')
                HandleTopicMode(client, channel, modes[0]);
            else if (modes[i] == 'k')
                HandleKeyMode(client, channel, iss, modes[0]);
            else if (modes[i] == 'o')
                HandleOperatorMode(client, channel, iss, modes[0]);
            else if (modes[i] == 'l')
                HandleLimitMode(client, channel, iss, modes[0]);
            else
                client->SendData("MODE :Invalid mode : " + modes[i] + '\n');
        }
    }
    iss >> trash;
    if (trash.empty() == false)
    	while(iss.get() != '\n');
}