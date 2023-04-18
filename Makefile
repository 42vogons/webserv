# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/27 00:08:05 by cpereira          #+#    #+#              #
#    Updated: 2023/04/17 00:49:27 by anolivei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC_DIR = srcs

OBJ_DIR = .objs

SRC =	$(SRC_DIR)/main.cpp \
		$(SRC_DIR)/Socket.cpp \
		$(SRC_DIR)/Sockets.cpp \
		$(SRC_DIR)/Server.cpp \
		$(SRC_DIR)/Servers.cpp \
		$(SRC_DIR)/LocationServer.cpp

OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.opp, $(SRC))

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow -g -fsanitize=address
RM = /bin/rm -rf

all: hosts $(NAME)

$(NAME): $(OBJ)
		@$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

$(OBJ_DIR)/%.opp: $(SRC_DIR)/%.cpp
		@mkdir -p $(OBJ_DIR)
		@$(CC) $(CFLAGS) -c $< -o $@
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;199mCompiling\033[0m $(<F)"

clean:
		@$(RM) $(OBJ_DIR)
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRemoving objects\033[0m"

fclean: clean
		@$(RM) $(NAME)
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRemoving $(NAME)\033[0m"
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRestoring /etc/hosts\033[0m"
		@sudo cp ./hosts_backup /etc/hosts

hosts:
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;199mMaking a backup of\033[0m /etc/hosts"
		@cp /etc/hosts ./hosts_backup

re: fclean all

.PHONY: all clean fclean hosts re
