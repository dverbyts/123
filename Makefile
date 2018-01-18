NAME = dns
CLAGS = -Wall -Wextra -Werror
CC = gcc
SRC = get_next_line.c read.c main.c
OBJ = $(SRC:.c=.o)

all: $(SRC) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all