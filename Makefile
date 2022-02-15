CC=clang++
CFLAGS=-Wall -Wextra -Werror -std=c++98 -g3 -fsanitize=address -I .
NAME=tests
HEADS=algorithm.hpp pair.hpp reverse_iterator.hpp type_traits.hpp vector.hpp map.hpp
SRCS=main.cpp
OBJS=$(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS) $(HEADS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
