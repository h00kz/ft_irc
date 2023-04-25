# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ffeaugas <ffeaugas@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/28 16:38:05 by jlarrieu          #+#    #+#              #
#    Updated: 2023/04/24 17:57:49 by pdubacqu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		=	c++
CFLAGS	=	-g -Wall -Wextra -Werror -std=c++98

SRCS	=	src/main.cpp					\
			src/Client.cpp					\
			src/Channel.cpp					\
			src/Server/Server.cpp			\
			src/Server/cmd/Join.cpp			\
			src/Server/cmd/List.cpp			\
			src/Server/cmd/Mode.cpp			\
			src/Server/cmd/Nick.cpp			\
			src/Server/cmd/Part.cpp			\
			src/Server/cmd/Pass.cpp			\
			src/Server/cmd/Ping.cpp			\
			src/Server/cmd/Pong.cpp			\
			src/Server/cmd/PrivMsg.cpp		\
			src/Server/cmd/Topic.cpp		\
			src/Server/cmd/Invite.cpp		\
			src/Server/cmd/Kick.cpp			\
			src/Server/cmd/User.cpp		

OBJ			= $(SRCS:.cpp=.o)
NAME	=	ircserv
RM		=	rm -f
bold	:= $(shell tput bold)
green	:= $(shell tput setaf 2)
sgr0	:= $(shell tput sgr0)

all: $(NAME)

.cpp.o: $(SRCS)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	@$(CC) ${OBJ} -o $(NAME) $(CFLAGS)
	@printf "[$(green)✔$(sgr0)] $(bold)$(green)Compiling ircserv finished!$(sgr0)\n"

clean:
			@echo "-----  Cleaning all objects...  -----"
			@${RM} ${OBJ}

fclean:		clean
			@${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re bonus
