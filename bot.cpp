/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/06 18:40:51 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/10 13:40:37 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <ctime>

int randomNumber(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void	gamePFC(bool &gameRun, int clientSd, std::string const& sender, std::string const& aiChoice, std::string const& choice)
{
	std::string formatMsg;

	if (strstr(aiChoice.c_str(), "ciseaux") != NULL)
	{
		if (strstr(choice.c_str(), "feuille") != NULL)
			formatMsg = "PRIVMSG " + sender + " :BotMan a gagné! BotMan a choisit " + aiChoice + "\r\n";
		if (strstr(choice.c_str(), "pierre") != NULL)
			formatMsg = "PRIVMSG " + sender + " :Vous avez gagné! BotMan a choisit " + aiChoice + "\r\n";
		if (strstr(choice.c_str(), "ciseaux") != NULL)
			formatMsg = "PRIVMSG " + sender + " :Egalité!\r\n";
		send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
	}
	else if (strstr(aiChoice.c_str(), "pierre") != NULL)
	{
		if (strstr(choice.c_str(), "feuille") != NULL)
			formatMsg = "PRIVMSG " + sender + " :Vous avez gagné! BotMan a choisit " + aiChoice + "\r\n";
		if (strstr(choice.c_str(), "pierre") != NULL)
			formatMsg = "PRIVMSG " + sender + " :Egalité!\r\n";
		if (strstr(choice.c_str(), "ciseaux") != NULL)
			formatMsg = "PRIVMSG " + sender + " :BotMan a gagné! BotMan a choisit " + aiChoice + "\r\n";
		send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
	}
	else if (strstr(aiChoice.c_str(), "feuille") != NULL)
	{
		if (strstr(choice.c_str(), "feuille") != NULL)
			formatMsg = "PRIVMSG " + sender + " :Egalité!\r\n";
		if (strstr(choice.c_str(), "pierre") != NULL)
			formatMsg = "PRIVMSG " + sender + " :BotMan a gagné! BotMan a choisit " + aiChoice + "\r\n";
		if (strstr(choice.c_str(), "ciseaux") != NULL)
			formatMsg = "PRIVMSG " + sender + " :Vous avez gagné! BotMan a choisit " + aiChoice + "\r\n";
		send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
	}
	gameRun = false;
}

void	gamePM(bool &gameRun, std::string const& sender, int clientSd, int &targetNumber, int guess)
{
	std::string formatMsg;
	if (guess == targetNumber)
	{
		formatMsg = "PRIVMSG " + sender + " :Félicitations! Vous avez deviné le bon nombre!\r\n";
		send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
		gameRun = false;
		targetNumber = randomNumber(1, 100);
	}
	else if (guess < targetNumber)
	{
		formatMsg = "PRIVMSG " + sender + " :Le nombre que vous avez deviné est trop petit.\r\n";
		send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
	}
	else
	{
		formatMsg = "PRIVMSG " + sender + " :Le nombre que vous avez deviné est trop grand.\r\n";
		send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
	}
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		std::cerr << "Usage: ./bot <ip_address> <port> <password>" << std::endl;
		exit(1); 
	}
	srand(time(NULL));
	int targetNumber;
	bool gameRunningPM = false;
	bool gameRunningPFC = false;
	bool menuDisplayed = false;
	char *serverIp = argv[1];
	int port = std::atoi(argv[2]);
	std::string passCmd = "PASS " + std::string(argv[3]) + "\r\n";
	if (port < 1 || port != static_cast<uint16_t>(port))
	{
		std::cerr << "Port need to be between 1 and 65635" << std::endl;
		exit(1);
	}
	char msg[4096]; 
	sockaddr_in sendSockAddr;
	bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
	sendSockAddr.sin_family = AF_INET;
	sendSockAddr.sin_addr.s_addr = inet_addr(serverIp);
	sendSockAddr.sin_port = htons(port);
	int clientSd = socket(AF_INET, SOCK_STREAM, 0);
	int status = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
	if (status < 0)
	{
		std::cout<<"Error connecting to socket!"<<std::endl;
		return -1;
	}
	std::cout << "Connected to the server!" << std::endl;
	size_t bytesRecv;
	send(clientSd, passCmd.c_str(), strlen(passCmd.c_str()), 0);
	usleep(200);
	send(clientSd, "NICK BotMan\r\n", strlen("NICK BotMan\r\n"), 0);
	usleep(200);
	send(clientSd, "USER BotMan localhost 127.0.0.1 :therealbotman\r\n", strlen("USER BotMan localhost 127.0.0.1 :therealbotman\r\n"), 0);
	usleep(200);
	while (true)
	{
		bzero(&msg, sizeof(msg));
		if ((bytesRecv = recv(clientSd, (char*)&msg, sizeof(msg) - 1, 0)) == 0)
			break;
		msg[bytesRecv] = '\0';
		if (strstr(msg, "PRIVMSG") != NULL)
		{
			size_t pos;
			std::string req(msg);
			std::string sender, message;
			pos = req.find_first_of(" ");
			sender = req.substr(1, pos);
			pos = req.find_last_of(":");
			message = req.substr(pos + 1, req.find_last_not_of("\r") + 1);
			std::string formatMsg;
			if (!gameRunningPM && !gameRunningPFC)
			{
				formatMsg = "PRIVMSG "+ sender + " :Tapez !menu pour afficher le menu\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				menuDisplayed = true;
			}
			if (gameRunningPM && !menuDisplayed)
			{
				int guess;

				try {
					guess = std::atoi(message.c_str());
					std::cout << targetNumber << std::endl;
				}
				catch (const std::invalid_argument& e) {
					continue;
				}
				gamePM(gameRunningPM, sender, clientSd, targetNumber, guess);
			}
			else if (gameRunningPFC && !menuDisplayed)
			{
				std::string choice;
				std::string aiChoice;
				std::string choices[3] = {"pierre", "feuille", "ciseaux"};

				targetNumber = randomNumber(0, 2);
				aiChoice = choices[targetNumber];
				try {
					choice = message.substr(message.find_first_not_of(" "), message.find_last_not_of("\r\n") + 1);
					std::cout << choice << std::endl;
				}
				catch (const std::invalid_argument& e) {
					continue;
				}
				gamePFC(gameRunningPFC, clientSd, sender, aiChoice, choice);
			}
			else if (menuDisplayed && strstr(message.c_str(), "!menu") != NULL && !gameRunningPM && !gameRunningPFC)
			{
				formatMsg = "PRIVMSG " + sender + " :Menu:\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				formatMsg = "PRIVMSG " + sender + " :1. Jouer au jeu du plus ou moins (1 et 100)\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				formatMsg = "PRIVMSG " + sender + " :2. Jouer a Pierre, Feuille, Ciseaux\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				formatMsg = "PRIVMSG " + sender + " :3. Quitter\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				formatMsg = "PRIVMSG " + sender + " :Entrez le numéro de l'option que vous souhaitez choisir.\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
			}
			if (menuDisplayed && strstr(message.c_str(), "1") != NULL && !gameRunningPM && !gameRunningPFC)
			{
				gameRunningPM = true;
				formatMsg = "PRIVMSG " + sender + " :Le jeu du plus ou moins commence maintenant !\r\n";
				targetNumber = randomNumber(1, 100);
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				menuDisplayed = false;
			}
			else if (menuDisplayed && strstr(message.c_str(), "2") != NULL  && !gameRunningPM && !gameRunningPFC)
			{
				gameRunningPFC = true;
				formatMsg = "PRIVMSG " + sender + " :Le jeu Pierre, Feuille, Ciseaux commence maintenant !\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				menuDisplayed = false;
			}
			else if (menuDisplayed && strstr(message.c_str(), "3") != NULL && !gameRunningPM && !gameRunningPFC)
			{
				formatMsg = "PRIVMSG " + sender + " :Au revoir !\r\n";
				send(clientSd, formatMsg.c_str(), formatMsg.length(), 0);
				menuDisplayed = false;
				continue;
			}
		}
		if (!strcmp(msg, "PING"))
		{
			send(clientSd, "PONG 127.0.0.1\r\n", strlen("PONG 127.0.0.1\r\n"), 0);
		}
	}
	close(clientSd);
	return 0;
}