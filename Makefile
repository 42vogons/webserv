# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/27 00:08:05 by cpereira          #+#    #+#              #
#    Updated: 2023/04/07 19:12:25 by cpereira         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test.out

ODIR =	./objs/

SRCS =	main.cpp 

OBJS =	$(patsubst %.c, $(ODIR)%.o, $(SRCS))

CC = clang++
C_FLAGS = -Wall -Werror -Wextra -std=c++98


$(NAME): $(OBJS)
		@$(CC) $(OBJS) $(C_FLAGS) -g -o $@
		@echo "\033[1;34mDone\033[0;37m"

$(ODIR)%.o: $(SDIR)%.c
		@mkdir -p $(ODIR)
		@$(CC) $(C_FLAGS) -c $< -o $@


all: karen

clean:
	@rm -rf $(ODIR)

fclean: clean
	@rm -f $(NAME)
	@echo "\033[1;33mCleaned\033[0;37m"

re: fclean all
