################################### IRC ###################################

EXE := ircserv
BEXE := ircbot

CPP := c++
# -std=c++98

CPPFLAGS := -Wall -Wextra -Wshadow -MMD #-g -fsanitize=address
# -Werror

################################### SRCS ###################################

DIR := ./Compiled

FILES := ./Mandatory/src/client/client.cpp \
         ./Mandatory/src/server/server.cpp \
         ./Mandatory/src/server/utils.cpp \
         ./Mandatory/src/channel/channel.cpp \
         ./Mandatory/src/commands/additionalCommands.cpp \
         ./Mandatory/src/commands/channelOpsCommands.cpp \
         ./Mandatory/src/commands/joinCommand.cpp \
         ./Mandatory/src/commands/authenticationCommands.cpp \
         ./Mandatory/src/commands/modeCommand.cpp \
         ./Mandatory/src/tools/parse.cpp \
         ./Mandatory/src/tools/utils.cpp \
         ./Mandatory/main.cpp

BFILES := ./Bonus/src/bot.cpp \
          ./Bonus/src/commands.cpp \
          ./Bonus/src/tools.cpp \
          ./Bonus/main.cpp

#############################################################################

OBJ := $(FILES:%.cpp=$(DIR)/%.o)
BOBJ := $(BFILES:%.cpp=$(DIR)/%.o)

DEPS := $(OBJ:$(DIR)/%.o=$(DIR)/%.d) \
        $(BOBJ:$(DIR)/%.o=$(DIR)/%.d)

M = MAKE_PUSH

################################### RULES ###################################

all: $(EXE)

bonus: $(BEXE)

$(EXE): $(OBJ)
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(EXE)

$(BEXE): $(BOBJ)
	$(CPP) $(CPPFLAGS) $(BOBJ) -o $(BEXE)

$(DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(DIR)

fclean: clean
	rm -rf $(EXE) $(BEXE)

re: fclean all

-include $(DEPS)

push: fclean
	git add .
	git status
	git commit -m "$(M)"
	git push

##############################################################################

.PHONY: clean fclean re push bonus
