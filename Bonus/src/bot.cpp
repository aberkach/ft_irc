#include "../Inc/ircBot.hpp"

Bot::Bot(void)
{
    std::string master;

    std::cout << "whats ur server Nickname master :";
    getline(std::cin, master);
    if(master.empty()) // or has spaces
        throw (std::invalid_argument( "ERROR: Nickname cant be empty nor have eny spaces in it."));

    std::cout << YELLOW "your Nickname is :" << master << RESET << std::endl;
    masters.push_back(master);
    std::cout << "Do you want to continue? [Y/n] ";
    getline(std::cin, master);
    if (master.empty() || (master != "Y" && master != "y"))
        throw (std::invalid_argument( "ERROR: " BOT ": Quit." ));

    std::cout << "Do u have an API_KEY from api.openweathermap.org? [Y/n] ";
    if (getline(std::cin, master) && !master.empty() && (master == "Y" || master == "y"))   // optimize this
    {
        std::cout << "API_KEY :";
        getline(std::cin, master);
        if(master.empty()) { // or has spaces
            std::cout << YELLOW "API_KEY cant be empty nor have eny spaces in it." << std::endl;
            std::cout << "API will be set to null u can configure it later." RESET << std::endl; // info
            this->API_KEY = "\0";
        } else
            this->API_KEY = master;
    } else {
        std::cout << YELLOW "API will be set to null u can configure it later." RESET << std::endl; // info
        this->API_KEY = "\0";
    }

    if ((this->irc_sock = createTCPSock()) == -1)
        throw (std::invalid_argument( "ERROR: Failed to create socket."));

    commandList["PING"] = &Bot::pongCommand;
    commandList["JOIN"] = &Bot::welcomeMsg;
    commandList["PART"] = &Bot::leaveMsg;
    commandList["/help"] = &Bot::helpCommand;
    commandList["/joke"] = &Bot::jokeCommand;
    commandList["/toss"] = &Bot::tossCommand;
    commandList["/weather"] = &Bot::weatherCommand;
    commandList["/admin"] = &Bot::adminCommand;
};

void
Bot::authenticate(std::string password) const
{
    int i = 0;
    std::string response;
        std::string commands[3] = {
                                "PASS " + password,
                                "NICK " BOT,
                                "USER " BOT " 0 * :" BOT,
                              };

    if (password.empty())
        i++;

    for (; i <= 2; i++) {
        sendit(this->irc_sock, commands[i]);
        if ((response = recvit(this->irc_sock)) == "")
            continue;
        logger(response);
    }
    sleep(1);
    logger(recvit(this->irc_sock));
};

void
Bot::logger(const std::string &message) const
{
    int code;
    std::string output[2];
    std::istringstream stream(message);

    stream >> output[0];
    stream >> code;
    stream >> output[1];

    if __FATAL(code) {
        close(this->irc_sock);
        throw (std::invalid_argument(message));
    } else if __NONFATAL(output[0]) {
        output[0] = "PONG " + output[1];
        sendit(this->irc_sock, output[0]);
    }
    else
        __LOG(message, GREEN)
};

std::vector<std::string>
Bot::getBuffers(const std::string &buffer) {
    std::vector<std::string> messages;
    size_t start = 0, end = 0;

	while ((end = buffer.find_first_of('\n',start)) != std::string::npos) {
		while (buffer[end] && buffer[end] == '\n')
			end++;
		std::string message = buffer.substr(start, end - start);
		messages.push_back(message);
		start = end;
	}

    if (start < buffer.length())
        messages.push_back(buffer.substr(start));

    return messages;
};

static void
replaceChar(std::string &str, char oldChar, char newChar) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == oldChar) {
            str[i] = newChar;
        }
    }
}

void
Bot::listenForCommand(void)
{
    const char *MAN[] = MANUAL;
    const int    size = sizeof(MAN) / sizeof(MAN[0]);

    std::cout << "=> Bot nick name    : " << BOT << std::endl;
    std::cout << "=> init master name : " << this->masters.at(0) << '\n' << std::endl;
    std::cout << "==> Available commands:" << std::endl;
    for (int i = 0; i < size; i++)
        std::cout << "  " << MAN[i];

    while (true)
    {
        std::string message;
        if ((message = recvit(this->irc_sock)) == "")
            continue;
		replaceChar(message, '\r', '\n');
        std::vector<std::string> messages = getBuffers(message);
        logger(message);
        for (std::vector<std::string>::iterator it = messages.begin(); it != messages.end(); ++it)
        {
            *it = trimTheSpaces(message);
            std::vector<std::string> fields = splitMessage(message);
            if (fields.empty())
                continue;

            if (fields[0] == "PING")
                (this->*commandList[fields[0]])(fields);
            else if (fields.size() > 1 && commandList.find(fields[1]) != commandList.end())
                (this->*commandList[fields[1]])(fields);
            else if (fields.size() > 2 && fields[1] == "PRIVMSG" && commandList.find(fields[3]) != commandList.end())
                (this->*commandList[fields[3]])(fields);
        }
    }
};

bool
Bot::masterStatus(const std::string &name)
{
    std::string command;

    for (std::vector<std::string>::iterator it = masters.begin(); it != masters.end(); ++it) {
        if (name == *it)
            return true;
    }

    command = "PRIVMSG " + name + SUDOERROR;
    sendit(this->irc_sock, command);
    return false;
};

void
Bot::startBot(uint16_t port)
{
    std::string answer;

    std::cout << YELLOW "connect using :\n     1 - using LOCALHOST.\n     2 - using ip address.\n     3 - using domain name.\n" RESET << std::endl; // hardcoded for now
    std::cout << "Answer [1/2/3] ";

    getline(std::cin, answer);
    if (answer == "1") {
        if (!connectSockByIp(this->irc_sock, LOCALHOST, port))
            throw (std::invalid_argument( "ERROR: " RED "Couldnt connect to socket address." RESET));
    } else if (answer == "2") {
        std::cout << "Irc Server ip: ";
        getline(std::cin, answer);
        if (!connectSockByIp(this->irc_sock, answer.c_str(), port))
            throw (std::invalid_argument( "ERROR: " RED "Couldnt connect to socket address." RESET));
    } else if (answer == "3") {
        std::cout << "Irc server name: ";
        getline(std::cin, answer);
        if (!connectSockByName(this->irc_sock, answer.c_str(), port))
            throw (std::invalid_argument( "ERROR: " RED "Couldnt connect to socket address." RESET));
    } else {
        close(this->irc_sock);
        throw (std::invalid_argument( "ERROR: " RED "Only [1/2/3] are allowed." RESET));
    }
};

Bot::~Bot()
{
    close(this->irc_sock);
};
