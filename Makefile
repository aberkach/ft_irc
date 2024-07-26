################################### IRC ###################################

EXE := ircserv
BEXE := ircbot

CPP := c++

CPPFLAGS := -g -Wall -Wextra -Wshadow -fsanitize=address
# -Werror

################################### SRCS ###################################
# HEADER := ./srcs/server/server.hpp		 ./srcs/server/client.hpp					  ./srcs/tools/health.hpp			     \
		#   ./Inc/ft_irc.hpp				 ./Inc/define.hpp							  ./srcs/channel/channel.hpp
FILES := ./srcs/client/client.cpp		 ./srcs/server/server.cpp 					  ./srcs/server/utils.cpp				 \
		./srcs/channel/channel.cpp		 ./srcs/commands/additionalCommands.cpp		  ./srcs/commands/channelOpsCommands.cpp \
		./srcs/commands/joinCommand.cpp	 ./srcs/commands/authenticationCommands.cpp	  ./srcs/commands/modeCommand.cpp 		 \
		./srcs/tools/health.cpp			 ./srcs/main.cpp 

# BHEADER := ./bonus/include/defines.hpp 	./bonus/Inc/ircBot.hpp 	 ./bonus/bot/bot.hpp 
BFILES :=  ./bonus/bot/bot.cpp 		./bonus/bot/commands.cpp ./bonus/bot/tools.cpp ./bonus/main.cpp

#############################################################################

OBJ := $(FILES:.cpp=.o)

BOBJ := $(BFILES:.cpp=.o)


M = MAKE_PUSH

################################### RULES ###################################

all : $(EXE)

bonus : $(BEXE)

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ)  -o $(EXE)

$(BEXE): $(BOBJ)
	$(CPP) $(CPPFLAGS) $(BOBJ) -o $(BEXE)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean :
	rm -rf $(BOBJ) $(OBJ) 

fclean : clean
	rm -rf $(EXE) $(BEXE)

re: fclean all

push: fclean
	git add .
	git status
	git commit -m "$(M)"
	git push

##############################################################################

.PHONY:  clean fclean re push bonus