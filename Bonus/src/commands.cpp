#include "../Inc/ircBot.hpp"

void
Bot::jokeCommand(const std::vector<std::string> &fields)
{
    const char* jokes[] = JOKES;

    const int numJokes = sizeof(jokes) / sizeof(jokes[0]);
    const std::string &client = __GETARGET;
    std::string command = "PRIVMSG " + client + " :";

    srand(static_cast<unsigned>(time(0)));

    int randomIndex = rand() % numJokes;

    command += jokes[randomIndex];
    sendit(this->irc_sock, command);
};

void
Bot::tossCommand(const std::vector<std::string> &fields)
{
    unsigned int result;
    std::string command;
    const std::string &client = __GETARGET;

    srand(time(0));
    result = rand() % 2;

    command = "PRIVMSG " + client + " :flipping a coin ...";
    sendit(this->irc_sock, command);

    command = "PRIVMSG " + client + ((result == 0) ? " :Heads !!!" : " :Tails !!!");
    sendit(this->irc_sock, command);
};

const std::map<std::string, std::string>
Bot::extractWeatherData(const std::string &response)
{
    std::map<std::string, std::string> weatherData;

    weatherData["longitude"] = jsonValue(response, "lon");
    weatherData["latitude"] = jsonValue(response, "lat");
    weatherData["main"] = jsonValue(response, "main");
    weatherData["description"] = jsonValue(response, "description");
    weatherData["temperature"] = jsonValue(response, "temp");
    weatherData["feels_like"] = jsonValue(response, "feels_like");
    weatherData["pressure"] = jsonValue(response, "pressure");
    weatherData["humidity"] = jsonValue(response, "humidity");
    weatherData["visibility"] = jsonValue(response, "visibility");
    weatherData["wind_speed"] = jsonValue(response, "speed");
    weatherData["wind_deg"] = jsonValue(response, "deg");
    weatherData["clouds"] = jsonValue(response, "all");
    weatherData["country"] = jsonValue(response, "country");
    weatherData["city"] = jsonValue(response, "name");

    return weatherData;
};

std::string
Bot::getWeatherData(const std::map<std::string, std::string>& weatherData , const std::string &client)
{
    std::ostringstream message;

    try {
        message << "PRIVMSG " << client << " :Weather report for " << weatherData.at("city") << ", " << weatherData.at("country") << ":\r\n";
        message << "PRIVMSG " << client << " :Coordinates: (" << weatherData.at("latitude") << ", " << weatherData.at("longitude") << ")\r\n";
        message << "PRIVMSG " << client << " :Weather: " << weatherData.at("main") << " (" << weatherData.at("description") << ")\r\n";
        message << "PRIVMSG " << client << " :Temperature now: " << std::stof(weatherData.at("temperature")) - 273.15 << "°C (feels like " << std::stof(weatherData.at("feels_like")) - 273.15 << "°C)\r\n";
        message << "PRIVMSG " << client << " :Pressure: " << weatherData.at("pressure") << " hPa\r\n";
        message << "PRIVMSG " << client << " :Humidity: " << weatherData.at("humidity") << "%\r\n";
        message << "PRIVMSG " << client << " :Visibility: " << weatherData.at("visibility") << " meters\r\n";
        message << "PRIVMSG " << client << " :Wind: " << weatherData.at("wind_speed") << " m/s at " << weatherData.at("wind_deg") << "°\r\n";
        message << "PRIVMSG " << client << " :Cloudiness: " << weatherData.at("clouds") << "%\r\n";
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return (message.str());
};

void
Bot::weatherCommand(const std::vector<std::string> &fields)
{
    std::string command;
    const std::string &client = __GETARGET;

    if (fields.size() > 4)
    {
        int weather_sock;
        int bytes = 0;
        char buffer[4096];

        if ((weather_sock = createTCPSock()) == -1 || !connectSockByName(weather_sock, HOST, PORT))
        {
            command = "PRIVMSG " + client + " :failed to connect to " HOST " server";
            return;
        }

        std::string response;
        std::string request = "GET /data/2.5/weather?q=" + fields[4] + "&appid=" + API_KEY + " HTTP/1.1\r\n";
        request += "Host: " HOST "\r\n";
        request += "Connection: close\r\n";

        sendit(weather_sock, request);

        while ((bytes = recv(weather_sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytes] = '\0';
            response += buffer;
        }

        if (bytes < 0)
            command = "PRIVMSG " + client + " :failed to get weather Data from " HOST " server";
        else if (jsonValue(response, "cod") == "200") {
            std::map<std::string, std::string> weatherData = extractWeatherData(response);
            command = getWeatherData(weatherData, client);
        }
        else
            command = "PRIVMSG " + client + " :/weather " + jsonValue(response, "message");
        close(weather_sock);
    } else {
        command = "PRIVMSG " + client + " :/weather missing argument.";
    }
    sendit(this->irc_sock, command);
};

// void
// bot::pollCommand(const std::vector<std::string> &fields) {};

void
Bot::helpCommand(const std::vector<std::string> &fields)
{
    std::string command;
    const char *manual[] = MANUAL;
    const std::string &client = __GETARGET;  // make a loop of 200ms delay between msgs to avoide irc bans

    int total = sizeof(manual) / sizeof(manual[0]);

    for (int i = 0; i < total; i++) {
        command += "PRIVMSG " + client + manual[i] + "\r\n";
    }
    sendit(this->irc_sock, command);
};

void
Bot::welcomeMsg(const std::vector<std::string> &fields)
{
    if (fields[0] == BOT) {
        std::string channel = fields[2].substr(1);
        for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
            if (channel == *it)
                return;
        }
        channels.push_back(fields[2].substr(1));
        return;
    }

    std::string command = "PRIVMSG " + fields[0] + WELCOME(fields[2]);
    sendit(this->irc_sock, command);
};

void
Bot::leaveMsg(const std::vector<std::string> &fields)
{
    if (fields[0] == BOT)
        return;

    std::string command = "PRIVMSG " + fields[0] + FAREWELL(fields[2]);
    sendit(this->irc_sock, command);
};

void
Bot::pongCommand(const std::vector<std::string> &fields)
{
    std::string command = "PONG " + ((fields.size() > 2) ? fields[1] : fields[2]);
    sendit(this->irc_sock, command);
};

void
Bot::adminCommand(const std::vector<std::string> &fields)
{
    std::string command;
    const std::string &action = fields[4];
    std::vector<std::string>::iterator it;

    if (!masterStatus(fields[0]))
        return;

    if (fields.size() > 5)
    {
        if (action == "join") {
            if ((it = std::find(channels.begin(), channels.end(), fields[5])) == channels.end()) {
                if (fields.size() == 6)
                    command = "JOIN " + fields[5];
                else
                    command = "JOIN " + fields[5] + fields[6];
                sendit(this->irc_sock, command);
            }
        } else if (action == "part") {

            std::vector<std::string> part = splitByDelim(fields[5], ',');
            for (std::vector<std::string>::iterator chanIt = part.begin(); chanIt != part.end(); ++it) {
                if ((it = std::find(channels.begin(), channels.end(), *chanIt)) != channels.end()) {
                    command = "PART " + *chanIt;
                    channels.erase(it);
                    sendit(this->irc_sock, command);
                }
            }
        } else if (action == "add") {
            if ((it = std::find(masters.begin(), masters.end(), fields[5])) == masters.end()) {
                masters.push_back(fields[5]);
            }
        } else if (action == "del") {
            if ((it = std::find(masters.begin(), masters.end(), fields[5])) != masters.end()) {
                masters.erase(it);
            }
        } else if (action == "set") {
                API_KEY = fields[5];
        } else if (action == "list") {
            command = "PRIVMSG " + fields[0];
            if (fields[5] == "masters") {
                command += " :Masters =";
                for (it = masters.begin(); it != masters.end(); it++) {
                    command += " " + *it;
                }
            } else if (fields[5] == "channels") {
                command += " :Channels =";
                for (it = channels.begin(); it != channels.end(); it++) {
                    command += " " + *it;
                }
            } else
                command += SYNXERROR;
            sendit(this->irc_sock, command);
        } else {
            command = "PRIVMSG " + fields[0] + SYNXERROR;
            sendit(this->irc_sock, command);
        }
    } else {
        command = "PRIVMSG " + fields[0] + SYNXERROR;
        sendit(this->irc_sock, command);
    }
};

// void
// Bot::unavailbleCommand(const std::vector<std::string> &fields) {
//     const std::string &client = __GETARGET;
//     std::string commnad = "PRIVMSG " + client + " :" + fields[3] + "command unavailbe, contact a master";
// }
