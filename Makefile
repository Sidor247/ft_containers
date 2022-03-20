CC=clang++
CFLAGS=-Wall -Wextra -Werror -std=c++98 -fsanitize=address -g -I.
NAME=tests
HEADS=vector.hpp map.hpp stack.hpp utility.hpp TreeIterator.hpp TreeNode.hpp
SRC=main.cpp
OBJ=main.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ): $(SRC) $(HEADS)
	$(CC) $(CFLAGS) -c $(SRC)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
