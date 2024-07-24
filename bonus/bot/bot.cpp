#include "../include/ircBot.hpp"

Bot::Bot(void) 
{
    std::string master;

    std::cout << "whats ur server Nickname master :";
    getline(std::cin, master);
    if(master.empty()) // or has spaces
        throw (std::invalid_argument("Nickname cant be empty nor have eny spaces in it."));

    std::cout << YELLOW "your Nickname is :" << master << RESET << std::endl;
    masters.push_back(master);
    std::cout << "Do you want to continue? [Y/n] ";
    getline(std::cin, master);
    if (master.empty() || (master != "Y" && master != "y"))
        throw (std::invalid_argument(BOT ": Quit." ));

    std::cout << "Do u have an API_KEY from api.openweathermap.org? [Y/n] ";
    if (getline(std::cin, master) && !master.empty() && (master == "Y" || master == "y"))   // optimize this
    {
        std::cout << "API_KEY :";
        getline(std::cin, master);
        if(master.empty()) { // or has spaces
            std::cout << "API_KEY cant be empty nor have eny spaces in it." << std::endl;
            std::cout << "API will be set to null u can configure it later." << std::endl; // info
            this->API_KEY = "\0";
        } else 
            this->API_KEY = master;
    } else {
        std::cout << "API will be set to null u can configure it later." << std::endl; // info
        this->API_KEY = "\0";
    }

    if ((this->irc_sock = createTCPSock()) == -1)
        throw (std::invalid_argument("failed to create socket."));

    commandList["PING"] = &Bot::pongCommand;
    commandList["JOIN"] = &Bot::welcomeMsg;
    commandList["PART"] = &Bot::leaveMsg;
    commandList["/help"] = &Bot::helpCommand;
    commandList["/joke"] = &Bot::jokeCommand;
    commandList["/toss"] = &Bot::tossCommand;
    commandList["/weather"] = &Bot::weatherCommand;
    // commandList["/poll"] = &Bot::;
    commandList["/admin"] = &Bot::adminCommand;
};

void
Bot::authenticate(std::string password) const
{
    std::string message;

    if (password.empty())
        password = "NAN";

    message = "PASS " + password;

    sendit(this->irc_sock, message);

    message = "NICK " BOT;
    usleep(200);
    sendit(this->irc_sock, message);

    message = "USER " BOT " 0 * : " BOT;
    usleep(200);
    sendit(this->irc_sock, message);
};

void
Bot::logger(const std::string &message) const
{
    int code;
    std::string output;
    std::istringstream stream(message);

    stream >> output;
    stream >> code;

    if __FATAL(code) {
        close(this->irc_sock);
        throw (std::invalid_argument(message));
    } else if __NONFATAL(code)
        __LOG(message, YELLOW)
    else
        __LOG(message, GREEN)
};

void 
Bot::listenForCommand(void)
{
    while (true) {
        std::string message;
        if ((message = revcit(this->irc_sock)) == "")
            continue;

        message = trimTheSpaces(message);
        std::vector<std::string> fields = splitMessage(message);

        if (!fields.empty()) {
            logger(message);

            if (fields[0] == "PING") {
                (this->*commandList[fields[0]])(fields);
            } else if (fields.size() > 1 && commandList.find(fields[1]) != commandList.end()) {
                (this->*commandList[fields[1]])(fields);
            } else if (fields.size() > 2 && fields[1] == "PRIVMSG" && commandList.find(fields[3]) != commandList.end()) {
                (this->*commandList[fields[3]])(fields);
            }
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

    std::cout << "connect using :\n     1 - using LOCALHOST.\n     2 - using ip address.\n     3 - using domain name.\n" << std::endl; // hardcoded for now 
    std::cout << "Answer [1/2/3] ";

    getline(std::cin, answer);
    if (answer == "1") {
        if (!connectSockByIp(this->irc_sock, LOCALHOST, port))
            throw (std::invalid_argument("couldnt connect to socket address."));
    } else if (answer == "2") {
        std::cout << "Irc Server ip: ";
        getline(std::cin, answer);
        if (!connectSockByIp(this->irc_sock, answer.c_str(), port))
            throw (std::invalid_argument("couldnt connect to socket address."));
    } else if (answer == "3") {
        std::cout << "Irc server name: ";
        getline(std::cin, answer);
        if (!connectSockByName(this->irc_sock, answer.c_str(), port))
            throw (std::invalid_argument("couldnt connect to socket address."));
    } else {
        close(this->irc_sock);
        throw (std::invalid_argument("Only [1/2/3] are allowed."));
    }
};

Bot::~Bot()
{
    close(this->irc_sock);
};
