EXE := ircserv

CPP := c++ -std=c++98

CPPFLAGS := -Wall -Wextra -Wshadow
# -g -Werror stop rendring global useless in local

HEADER := $(FILE:.cpp=.hpp) ./Inc/ft_irc.hpp ./Inc/define.hpp \

FILE := ./srcs/client/client.cpp		./srcs/server/server.cpp 		./srcs/tools/health.cpp		./srcs/Poller/Poller.cpp \

OBJ := $(FILE:.cpp=.o)

M = MAKE_PUSH

################################### RULES ###################################

all : $(EXE)

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) ./srcs/main.cpp -o $(EXE)

%.o: %.cpp $(HEADER)
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean :
	rm -rf $(OBJ)

fclean : clean
	rm -rf $(EXE)

re: fclean all

push:
	git add .
	git status
	git commit -m "$(M)"
	git push

##############################################################################

.PHONY: all clean fclean re push