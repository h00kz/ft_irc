# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlarrieu <jlarrieu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/28 16:38:05 by jlarrieu          #+#    #+#              #
#    Updated: 2023/04/04 10:50:56 by jlarrieu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		=	c++
CFLAGS	=	-g3 -Wall -Wextra -Werror -std=c++98

SRCS	=	src/main.cpp \
			src/Client.cpp \
			src/Server.cpp 

OBJ			= $(SRCS:.cpp=.o)
NAME	=	irc
RM		=	rm -f
bold	:= $(shell tput bold)
green	:= $(shell tput setaf 2)
sgr0	:= $(shell tput sgr0)

all: $(NAME)

.cpp.o: $(SRCS)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(NAME): $(OBJ)
	@$(CC) ${OBJ} -o $(NAME) $(CFLAGS)
	@printf "[$(green)✔$(sgr0)] $(bold)$(green)Compiling Ft_irc finished!$(sgr0)\n"

clean:
			@echo "-----  Cleaning all objects...  -----"
			@${RM} ${OBJ}

fclean:		clean
			@${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re bonus