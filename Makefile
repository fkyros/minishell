# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gade-oli <gade-oli@student.42madrid.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/29 18:07:54 by gade-oli          #+#    #+#              #
#    Updated: 2025/03/29 18:38:11 by gade-oli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#colors----------------------------------------------------------

RED 	= '\033[1;31m'
GREEN   = '\033[1;32m'
BLUE    = '\033[1;34m'
RESET   = '\033[0;0m'

#variables-------------------------------------------------------

CC = cc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline

LIBFT_DIR = ./libft
SRC_DIR = ./src

SRCS = $(SRC_DIR)/cmdpath.c \
       $(SRC_DIR)/main.c \
       $(SRC_DIR)/parser_aux.c $(SRC_DIR)/parser.c \
       $(SRC_DIR)/utils.c \
       ${SRC_DIR}/builtins_1.c ${SRC_DIR}/builtins_2.c \
       ${SRC_DIR}/redirections.c ${SRC_DIR}/pipe_handling.c \
	   ${SRC_DIR}/memory_handler.c ${SRC_DIR}/command_parser.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=bin/%.o)

NAME = minishell

#recipes----------------------------------------------------------

all: $(NAME)

bin/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(NAME): $(OBJS)
	@echo $(BLUE)"compiling binaries..."$(RESET)
	@make -C $(LIBFT_DIR)
	$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)
	@echo $(GREEN)"$(NAME) compiled!"$(RESET)

clean:
	rm -f $(OBJS)
	@make clean -C $(LIBFT_DIR)
	@echo $(RED)"binaries deleted"$(RESET)

fclean: clean
	@make clean -C $(LIBFT_DIR)
	rm -f $(NAME)
	@echo $(RED)"$(NAME) deleted!"$(RESET)

re: fclean all

.PHONY: all clean fclean re
