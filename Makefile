CC=clang++
CFLAGS=-Wall -Wextra -Werror -std=c++98 -g3
NAME=tests
HEADS=Algorithm.hpp Pair.hpp ReverseIterator.hpp TypeTraits.hpp Vector.hpp
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
