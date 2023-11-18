# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/27 00:08:05 by cpereira          #+#    #+#              #
#    Updated: 2023/11/18 17:02:42 by anolivei         ###   ########.fr        #
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
CFLAGS = -Wall -Wextra -Werror -std=c++98 -Wshadow -g -fsanitize=address
RM = /bin/rm -rf
CP = /bin/cp

all: $(NAME)

$(NAME): $(OBJ)
		@$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

$(OBJ_DIR)/%.opp: $(SRC_DIR)/%.cpp
		@mkdir -p $(OBJ_DIR)
		@$(CC) $(CFLAGS) -c $< -o $@
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;199mCompiling\033[0m $(<F)"

backup:
	@if [ ! -e host_backup ]; then \
		$(CP) /etc/hosts host_backup; \
		echo "\033[0;32m[OK]\033[0m    \033[0;38;5;199mBackup created: host_backup\033[0m"; \
	else \
		echo "\033[0;31m[ERROR]\033[0m \033[0;38;5;196mBackup already exists\033[0m"; \
	fi

restore:
	@if [ -e host_backup ]; then \
		$(CP) host_backup /etc/hosts; \
		echo "\033[0;32m[OK]\033[0m    \033[0;38;5;199mBackup restored to /etc/hosts\033[0m"; \
	else \
		echo "\033[0;31m[ERROR]\033[0m \033[0;38;5;196mNo backup found\033[0m"; \
	fi

clean:
		@$(RM) $(OBJ_DIR)
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRemoving objects\033[0m"

fclean: clean restore
		@$(RM) $(NAME)
		@$(RM) host_backup
		@echo "\033[0;32m[OK]\033[0m    \033[0;38;5;44mRemoving $(NAME)\033[0m"

re: fclean all

run: backup all
	./$(NAME)

.PHONY: all clean fclean re backup restore run
