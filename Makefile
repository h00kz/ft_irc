# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/28 16:38:05 by jlarrieu          #+#    #+#              #
#    Updated: 2023/05/10 17:54:05 by jlarrieu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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
			src/Server/cmd/Notice.cpp		\
			src/Server/cmd/Topic.cpp		\
			src/Server/cmd/Invite.cpp		\
			src/Server/cmd/Kick.cpp			\
			src/Server/cmd/User.cpp
OBJ			=	$(SRCS:.cpp=.o)

SRC_BONUS	=	bot_bonus.cpp
OBJ_BONUS	=	$(SRC_BONUS:.cpp=.o)

CC			=	c++
CFLAGS		=	-g -Wall -Wextra -Werror -std=c++98 -O3
NAME		=	ircserv
NAME_BONUS	=	bot
RM			=	rm -f
bold		=	$(shell tput bold)
green		=	$(shell tput setaf 2)
sgr0		=	$(shell tput sgr0)

all: $(NAME)

%.o: %.cpp
		@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) ${OBJ} -o $(NAME) $(CFLAGS)
		@printf "[$(green)✔$(sgr0)] $(bold)$(green)Compiling ircserv finished!$(sgr0)\n"

$(NAME_BONUS): $(OBJ_BONUS)
		$(CC) $(CFLAGS) ${OBJ_BONUS} -o $(NAME_BONUS) $(CFLAGS)
		@printf "[$(green)✔$(sgr0)] $(bold)$(green)Compiling bot finished!$(sgr0)\n"

bonus: $(NAME) $(NAME_BONUS)
	
clean:
		@echo "-----  Cleaning all objects...  -----"
		@${RM} ${OBJ} ${OBJ_BONUS}

fclean: clean
		@${RM} ${NAME} ${NAME_BONUS}

re: fclean all

.PHONY:	all clean fclean re bonus