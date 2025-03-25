CC = gcc
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lreadline

LIBFT_DIR = ./libft
SRC_DIR = ./src

SRCS = $(SRC_DIR)/cmdpath.c \
       $(SRC_DIR)/main.c \
       $(SRC_DIR)/parser_aux.c $(SRC_DIR)/parser.c \
       $(SRC_DIR)/utils.c \
       ${SRC_DIR}/builtins_1.c ${SRC_DIR}/builtins_2.c \
       ${SRC_DIR}/redirections.c ${SRC_DIR}/pipe_handling.c

OBJS = $(SRCS:.c=.o)

NAME = minishell

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(LIBFT_DIR)
	$(CC) $(OBJS) -o $(NAME) -L$(LIBFT_DIR) -lft $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make fclean -C $(LIBFT_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
