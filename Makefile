################################### IRC ###################################

EXE := ircserv

BEXE := ircbot

CPP := c++ 
# -std=c++98

CPPFLAGS := -Wall -Wextra -Wshadow -g -fsanitize=address -MMD
# -Werror

################################### SRCS ###################################

FILES := ./Mandatory/src/client/client.cpp		 ./Mandatory/src/server/server.cpp 					  ./Mandatory/src/server/utils.cpp				 \
		 ./Mandatory/src/channel/channel.cpp		 ./Mandatory/src/commands/additionalCommands.cpp		  ./Mandatory/src/commands/channelOpsCommands.cpp \
		 ./Mandatory/src/commands/joinCommand.cpp	 ./Mandatory/src/commands/authenticationCommands.cpp	  ./Mandatory/src/commands/modeCommand.cpp 		 \
		 ./Mandatory/src/tools/parse.cpp			 ./Mandatory/src/tools/utils.cpp						  ./Mandatory/main.cpp

BFILES :=  ./Bonus/src/bot.cpp 		./Bonus/src/commands.cpp ./Bonus/src/tools.cpp		./Bonus/main.cpp

DEPS := $(FILES:.cpp=.d) $(BFILES:.cpp=.d)

#############################################################################

OBJ := $(FILES:.cpp=.o)

BOBJ := $(BFILES:.cpp=.o)

M = MAKE_PUSH

################################### RULES ###################################

-include $(DEPS)

all : $(EXE)

bonus : $(BEXE)

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ)  -o $(EXE)

$(BEXE): $(BOBJ)
	$(CPP) $(CPPFLAGS) $(BOBJ) -o $(BEXE)

%.o: %.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean :
	rm -rf $(BOBJ) $(OBJ) $(DEPS)

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