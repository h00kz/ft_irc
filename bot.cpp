#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#define PORT 8080

const std::string server_addr = "127.0.0.2";
const std::string channel = "#42";


void    ft_log_in()
{
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    std::string hello = "PASS coucou\r\n";
    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\n Socket creation error \n");
        return ;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<= 0) 
    {
        printf( "\nInvalid address/ Address not supported \n");
        return ;
    }
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
        printf("\nConnection Failed \n");
        return ;
    }
    send(client_fd, hello.c_str(), hello.length(), 0);
    hello = "NICK bot\r\n";
    sleep(1);
    send(client_fd, hello.c_str(), hello.length(), 0);
    hello = "USER bot bot bot :bot\r\n";
    sleep(1);
    send(client_fd, hello.c_str(), hello.length(), 0);
    sleep(1);
    hello = "JOIN #42\r\n";
    send(client_fd, hello.c_str(), hello.length(), 0);
    printf("Connection message sent\n");
}


int main() 
{
    ft_log_in();
    sockaddr_in _servAddr;
    int _epollFd;


    int _serverSd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSd == -1)
	{
		std::cerr << "Socket: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	int optval = 1;
	if (setsockopt(_serverSd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
	{
		std::cerr << "Sockopt: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	memset(&_servAddr, 0, sizeof(_servAddr));
	_servAddr.sin_family = AF_INET;
	_servAddr.sin_addr.s_addr = INADDR_ANY;
	_servAddr.sin_port = htons(1234);
	if (bind(_serverSd, reinterpret_cast<struct sockaddr*>(&_servAddr), sizeof(_servAddr)) == -1)
	{
		std::cerr << "Bind: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(_serverSd, 10) == -1)
	{
		std::cerr << "Listen: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	fcntl(_serverSd, F_SETFL, O_NONBLOCK);
	_epollFd = epoll_create1(0);
	if (_epollFd == -1)
	{
		std::cerr << "Epoll create: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.events = EPOLLIN;
	ev.data.fd = _serverSd;
	if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverSd, &ev) == -1)
	{
		std::cerr << "Epoll_ctl: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
    std::string nickname = "mybot";
    std::string message = "PASS coucou\r\n";
    // write(_serverSd, message.c_str(), message.length());
    // message = "NICK " + nickname + "\r\n";
    // write(_serverSd, message.c_str(), message.length());
    // message = "USER " + nickname + " 0 * :" + nickname + "\r\n";
    // write(_serverSd, message.c_str(), message.length());
    // message = "JOIN " + channel + "\r\n";
    // write(_serverSd, message.c_str(), message.length());
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) 
    {
        std::cerr << "Failed to create epoll instance" << std::endl;
        return -1;
    }
    struct epoll_event event;
    event.data.fd = _serverSd;
    event.events = EPOLLIN;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _serverSd, &event) < 0) 
    {
        std::cerr << "Failed to add socket to epoll" << std::endl;
        return -1;
    }
    const int max_events = 10;
    struct epoll_event events[max_events];
    while (true) 
    {
        int num_events = epoll_wait(epoll_fd, events, max_events, -1);
        if (num_events < 0) 
        {
            std::cerr << "epoll_wait failed" << std::endl;
            break;
        }
        for (int i = 0; i < num_events; i++) 
        {
            if (events[i].data.fd == _serverSd && (events[i].events & EPOLLIN)) 
            {
                char buffer[1024];
                std::cout << "coucou\n" << std::endl;
                int len = read(_serverSd, buffer, sizeof(buffer));
                std::cout << buffer << std::endl;
                if (len <= 0) 
                {
                    std::cerr << "Error reading from socket" << std::endl;
                    break;
                }
                buffer[len] = '\0';
                std::cout << buffer;
                std::string line(buffer);
                std::size_t pos = line.find("PRIVMSG " + channel);
                if (pos != std::string::npos) 
                {
                    pos = line.find(":");
                    if (pos != std::string::npos) 
                        std::cout << line.substr(pos + 1);
                }
            }
        }
    }
    std::cout << "coucou\n";
    close(_serverSd);
    return 0;
}
