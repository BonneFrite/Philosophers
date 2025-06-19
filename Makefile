CC		 = cc
CFLAGS	 = -Wall -Werror -Wextra -g3 #-fsanitize=address
NAME	 = philo

INC		 = -I ./includes

SRC_PATH = src/

SRC		= cleanup.c \
				main.c \
				parsing.c \
				philo.c \
				routines.c \
				time.c \
				utils.c \
				waiter.c

OBJ			= $(SRC:.c=.o)
SRCS = $(addprefix $(SRC_PATH), $(SRC))
OBJS	= $(addprefix $(SRC_PATH), $(OBJ))

all: $(OBJS) $(NAME)

$(SRC_PATH)%.o : $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
