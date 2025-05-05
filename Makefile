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

RED 	= \033[1;31m
GREEN   = \033[1;32m
BLUE    = \033[1;34m
RESET   = \033[0;0m
MAGENTA = \033[35m
BOLD    = \033[1m
PINK    = \033[38;5;206m

#variables-------------------------------------------------------

NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra -g
LDFLAGS = -lreadline

LIBFT_DIR = ./libft
SRC_DIR = ./src

SRCS = $(SRC_DIR)/cmdpath.c \
       $(SRC_DIR)/main.c \
       $(SRC_DIR)/parser_aux.c $(SRC_DIR)/parser.c \
       $(SRC_DIR)/utils.c \
       ${SRC_DIR}/builtins_1.c ${SRC_DIR}/builtins_2.c \
       ${SRC_DIR}/redirections.c ${SRC_DIR}/pipe_handling.c \
	   ${SRC_DIR}/memory_handler.c ${SRC_DIR}/command_parser.c \
	   ${SRC_DIR}/env/env.c ${SRC_DIR}/heredoc.c \
	   ${SRC_DIR}/process_handling.c ${SRC_DIR}/builtin_handling.c \
	   ${SRC_DIR}/heredoc_expand.c ${SRC_DIR}/parser_quote_handler.c \
	   

OBJS = $(SRCS:$(SRC_DIR)/%.c=bin/%.o)

#recipes----------------------------------------------------------

all: $(NAME)

bin/%.o: src/%.c
	@mkdir -p $(@D)
	@printf "${MAGENTA}Compiling ${CYAN}$<${RESET}\n"
	@$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIBFT_DIR) -c $< -o $@

$(NAME): $(OBJS)
	@printf "${BLUE}Linking binaries...${RESET}\n"
	@make -s -C $(LIBFT_DIR) --no-print-directory
	@$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)
	@printf "${BOLD}${GREEN}${NAME} ${PINK}compiled successfully!${RESET}\n\n"
	@printf "%b" "\
	$(BOLD)$(PINK)       __  $(GREEN) _____   _      $(PINK)__  \n\
	$(PINK)  _   / /$(GREEN)  / ____| | |     $(PINK)\\\\ \\\\ \n\
	$(PINK) (_) | |  $(GREEN)| (___   | |__    $(PINK)| |\n\
	$(PINK)     | |   $(GREEN)\\\\___ \\\\  | '_ \\\\   $(PINK)| |\n\
	$(PINK)  _  | |   $(GREEN)____) | | | | |  $(PINK)| |\n\
	$(PINK) (_) | |  $(GREEN)|_____/  |_| |_|  $(PINK)| |\n\
	$(PINK)      \\\\_\\\\                  /_/ $(RST)\n\n"

clean:
	@rm -rf bin
	@make -s -C $(LIBFT_DIR) clean --no-print-directory
	@printf "${RED}Cleaned object files!${RESET}\n"

fclean: clean
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean --no-print-directory
	@printf "${BOLD}${RED}${NAME} deleted!${RESET}\n"

re: fclean all

.PHONY: all clean fclean re
