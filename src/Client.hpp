/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 11:17:22 by ffeaugas          #+#    #+#             */
/*   Updated: 2023/05/05 11:23:38 by ffeaugas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <ctime>
# include <unistd.h>
# include <cstring>
# include <iostream>
# include <algorithm>
# include <map>

class Server;
class Channel;

class Client
{
	private:
		int 							_socketDescriptor;
		struct sockaddr_in 				_address;
		Server* 						_server;
		bool							_authenticated;
		std::string 					_receivedData;
		std::string 					_nickname;
		std::string 					_realName;
		std::string 					_hostName;
		std::string 					_serverName;
		std::string 					_username;
		std::map<std::string, Channel*>	_channels;
		time_t 							_lastActive;
	
	public:
		Client(int socketDescriptor, struct sockaddr_in address, Server* server);
		~Client();

		void	AddChannel(Channel* channel);
		void	JoinChannel(const std::string &channel);
		void	LeaveChannels(void);
		Channel	*FindChannel(const std::string &name);
		void	Close();

		int		ReceiveData();
		void	SendData(const std::string& data);
		void	SendMessage(const std::string &target, const std::string &message);
		void	UpdateLastActive();
		
		//Setters

		void SetAuthenticated(bool b);
		void SetNickname(const std::string &nickname);
		void SetRealname(const std::string &nickname);
		void SetHost(const std::string &nickname);
		void SetUsername(const std::string &username);
		void SetServer(const std::string &username);
		
		//Getters

		const std::string					&GetNickname() const;
		const std::string					&GetRealname() const;
		const std::string					&GetHost() const;
		std::string const					&GetUsername() const;
		std::string const					&GetServer() const;
		int 								GetSocketDescriptor() const;
		const std::string					&GetReceivedData() const;
		time_t								GetLastActive() const;
		std::map<std::string, Channel*>&	GetChannels();
		const struct sockaddr_in			&GetAddress() const;
		bool								IsInChannel(const std::string& channel) const;
		bool								IsConnected() const;
		bool								IsAuthenticated() const;
};

#endif //CLIENT_HPP