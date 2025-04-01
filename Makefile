RED=\033[31m
GREEN=\033[32m
CYAN=\033[36m
YELLOW=\033[33m
BLUE=\e[34m
MAGENTA=\033[35m
RESET=\033[0m

CC = gcc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline

LIBFT_DIR = ./libft
SRC_DIR = ./src

SRCS = $(SRC_DIR)/cmdpath.c \
       $(SRC_DIR)/main.c \
       $(SRC_DIR)/parser_aux.c $(SRC_DIR)/parser.c \
       $(SRC_DIR)/utils.c \
       $(SRC_DIR)/builtins_1.c $(SRC_DIR)/builtins_2.c \
       $(SRC_DIR)/redirections.c $(SRC_DIR)/pipe_handling.c \
       $(SRC_DIR)/memory_handler.c $(SRC_DIR)/command_parser.c

OBJS = $(SRCS:.c=.o)

NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(CYAN)Compiling Libft!$(RESET)"
	@make -C $(LIBFT_DIR) --no-print-directory > /dev/null
	@echo "$(CYAN)Compiling Minishell!$(RESET)"
	@$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)
	@echo "$(GREEN)Success!$(RESET)"

%.o: %.c
	@$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	@echo "$(MAGENTA)Cleaning...$(RESET)"
	@rm -f $(OBJS)
	@make clean -C $(LIBFT_DIR) --no-print-directory > /dev/null
	@echo "$(GREEN)Done!$(RESET)"

fclean: clean
	@echo "$(RED)Cleaning EVERYTHING!!$(RESET)"
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR) --no-print-directory > /dev/null
	@echo "$(GREEN)Done!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
