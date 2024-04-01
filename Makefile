NAME = ircserv

HEADER = ./server/includes/server.hpp ./server/includes/clients.hpp

SRCS = main.cpp ./server/srcs/server.cpp ./server/srcs/clients.cpp

CPP = c++ -std=c++98

CFLAGS = -Wall -Wextra -Werror# -g #-fsanitize=address,undefined

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp #$(HEADER)
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
