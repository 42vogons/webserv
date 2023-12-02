# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/27 00:08:05 by cpereira          #+#    #+#              #
#    Updated: 2023/12/02 15:47:43 by anolivei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRC_DIR = srcs

OBJ_DIR = .objs

SRC =	$(SRC_DIR)/main.cpp \
		$(SRC_DIR)/Socket.cpp \
		$(SRC_DIR)/WebServer.cpp \
		$(SRC_DIR)/Poll.cpp \
		$(SRC_DIR)/Server.cpp \
		$(SRC_DIR)/ServersMap.cpp \
		$(SRC_DIR)/LocationServer.cpp \
		$(SRC_DIR)/HandleRequest.cpp \
		$(SRC_DIR)/Pages.cpp \
		$(SRC_DIR)/Process.cpp \
		$(SRC_DIR)/Utils.cpp

OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.opp, $(SRC))

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow
RM = /bin/rm -rf
CP = /bin/cp

all: $(NAME)

$(NAME): backup $(OBJ)
		@$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

$(OBJ_DIR)/%.opp: $(SRC_DIR)/%.cpp
		@mkdir -p $(OBJ_DIR)
		@$(CC) $(CFLAGS) -c $< -o $@
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;199mCompiling\033[0m $(<F)"

backup:
	@if [ ! -e hosts_backup ]; then \
		$(CP) /etc/hosts hosts_backup; \
		echo "\033[0;32m[OK]\033[0m    \033[0;38;5;199mBackup created: hosts_backup\033[0m"; \
	else \
		echo "make: Nothing to be done for 'all'."; \
	fi

clean:
		@$(RM) $(OBJ_DIR)
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRemoving objects\033[0m"
		@$(RM) hosts_backup
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRemoving hosts_backup\033[0m"

fclean: clean
		@$(RM) $(NAME)
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRemoving $(NAME)\033[0m"

re: fclean all

.PHONY: all clean fclean re backup
