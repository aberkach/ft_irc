EXE := ircserv

CPP := c++ -std=c++98

CPPFLAGS := -g -Wall -Wextra -Wshadow
#  -Werror stop rendring global useless in local

HEADER := Inc/ft_irc.hpp Inc/define.hpp Inc/colors.hpp

FILE := srcs/main.cpp	\
		srcs/client/client.cpp		srcs/server/server.cpp 		srcs/tools/health.cpp \

# HEADER = ./server/includes/server.hpp ./server/includes/clients.hpp

# SRCS = main.cpp ./server/srcs/server.cpp ./server/srcs/clients.cpp

OBJ := $(FILE:.cpp=.o)

M = MAKE_PUSH

################################### RULES ###################################

all : $(EXE)

$(EXE): $(OBJ)
	$(CPP) $(OBJ) -o $(EXE)

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