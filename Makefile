# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gade-oli <gade-oli@student.42madrid.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/29 18:07:54 by gade-oli          #+#    #+#              #
#    Updated: 2025/05/09 19:38:40 by gade-oli         ###   ########.fr        #
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

SRCS = 	$(SRC_DIR)/main/main.c $(SRC_DIR)/main/utils.c ${SRC_DIR}/main/memory_handler.c ${SRC_DIR}/main/execution.c \
		$(SRC_DIR)/parsing/cmdpath.c $(SRC_DIR)/parsing/parser_aux.c $(SRC_DIR)/parsing/parser.c \
		${SRC_DIR}/parsing/parser_quote_handler.c ${SRC_DIR}/parsing/command_parser.c \
		${SRC_DIR}/builtins/builtin_handler.c ${SRC_DIR}/builtins/ft_cd.c ${SRC_DIR}/builtins/ft_echo.c ${SRC_DIR}/builtins/ft_env.c \
		${SRC_DIR}/builtins/ft_exit.c ${SRC_DIR}/builtins/ft_export_unset.c ${SRC_DIR}/builtins/ft_pwd.c \
		${SRC_DIR}/env/env.c ${SRC_DIR}/env/env_utils.c \
		${SRC_DIR}/redirs_pipes/process_handling.c ${SRC_DIR}/redirs_pipes/redirections.c ${SRC_DIR}/redirs_pipes/pipe_handling.c \
		${SRC_DIR}/redirs_pipes/process_handling_aux.c \
		${SRC_DIR}/redirs_pipes/heredoc/heredoc.c ${SRC_DIR}/redirs_pipes/heredoc/heredoc_expand.c ${SRC_DIR}/redirs_pipes/heredoc/raw_delim.c \
		${SRC_DIR}/safe_funcs/safe_chdir.c


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
	$(PINK)      \\\\_\\\\                  /_/ $(RESET)\n\n"

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
