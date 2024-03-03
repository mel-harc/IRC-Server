# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/29 18:13:50 by mel-harc          #+#    #+#              #
#    Updated: 2024/02/29 18:13:51 by mel-harc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

NAME_B = ircclient

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

CC = c++

SRC = 	IRC.cpp  server/Server.cpp client/Client.cpp  command/Command.cpp \
		Channel/Channel.cpp Channel/Join.cpp Channel/Cmd.cpp Channel/mode.cpp\
		command/PrivateMsg.cpp command/Pass.cpp command/Nick.cpp command/User.cpp\
		Channel/Invite.cpp Channel/Kick.cpp Channel/ErrHandle.cpp Channel/Topic.cpp\
		Channel/topicCmd.cpp Channel/kickCmd.cpp

SRC_B = bot/main.cpp bot/Bot.cpp

IND = server/Server.hpp client/Client.hpp command/Command.hpp command/PrivateMsg.hpp command/Pass.hpp \
		command/Nick.hpp command/User.hpp

IND_B = bot/Bot.hpp

OBJ = $(SRC:.cpp=.o)
OBJ_B = $(SRC_B:.cpp=.o)

all : $(NAME)

%.o : %.cpp $(IND)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	$(CC) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(NAME_B) : $(OBJ_B)
	$(CC) $(CXXFLAGS) $(OBJ_B) -o $(NAME_B)

clean :
	rm -rf $(OBJ) $(OBJ_B)

fclean : clean
	rm -rf $(NAME) $(NAME_B)

bonus : $(NAME_B)

re : fclean all bonus
