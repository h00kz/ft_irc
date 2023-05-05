/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:18:48 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 11:35:36 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		std::cerr << "Usage: ./bot ip_address port" << std::endl;exit(1); 
	}
	char *serverIp = argv[1];
	int port = std::atoi(argv[2]);
	if (port < 1 || port != static_cast<uint16_t>(port))
	{
		std::cerr << "Port need to be between 1 and 65635" << std::endl;
		exit(1);
	}
	char msg[1500]; 
	sockaddr_in sendSockAddr;
	bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
	sendSockAddr.sin_family = AF_INET;
	sendSockAddr.sin_addr.s_addr = INADDR_ANY;
	sendSockAddr.sin_port = htons(port);
	int clientSd = socket(AF_INET, SOCK_STREAM, 0);
	int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
	if(status < 0)
	{
		std::cout<<"Error connecting to socket!"<<std::endl;
		return -1;
	}
	std::cout << "Connected to the server!" << std::endl;
	send(clientSd, "PASS coucou\r\n", strlen("PASS coucou\r\n"), 0);
	usleep(200);
	send(clientSd, "NICK BotMan\r\n", strlen("NICK BotMan\r\n"), 0);
	usleep(200);
	send(clientSd, "USER BotMan localhost 127.0.0.1 :therealbotman\r\n", strlen("USER BotMan localhost 127.0.0.1 :therealbotman\r\n"), 0);
	usleep(200);
	while (true)
	{
		bzero(&msg, sizeof(msg));
		if (recv(clientSd, (char*)&msg, sizeof(msg), 0) == 0)
			break;
		if (strstr(msg, "PRIVMSG") != NULL)
		{
			if (strstr(msg, "BotMan") != NULL)
			{
				std::string req(msg);
				std::string formattedMessage = "PRIVMSG " + req.substr(1, req.find_first_of(' ')) + " :coucou\r\n";
				send(clientSd, formattedMessage.c_str(), formattedMessage.length(), 0);
				usleep(200);
			}
		}
		if (!strcmp(msg, "PING"))
		{
			send(clientSd, "PONG\r\n", strlen("PONG\r\n"), 0);
			usleep(200);
		}
	}
	close(clientSd);
	return 0;
}