
#include "../../Inc/irc.hpp"

void Err(const std::string &msg)
{
	std::cerr << msg << std::endl;
}

void replaceChar(std::string &str, char oldChar, char newChar) {
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == oldChar) {
            str[i] = newChar;
        }
    }
}

std::string
trimTheSpaces(const std::string &str)
{
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
};

std::string
stringUpper(const std::string &_str)
{
	std::string upper(_str);

    for (std::string::size_type i = 0; i < _str.size(); ++i) {
        upper[i] = ::toupper(_str[i]);
	}

	return(upper);
};

std::vector<std::string>
splitBySpace(const std::string &str)
{
    std::string part;
    std::vector<std::string> parts;
    std::istringstream iss(str);

    while (iss >> part) {

		if (!part.empty() && part[0] == ':')
        {
			std::string reset;
			part.erase(part.begin());
			getline(iss, reset);
			part += reset;
			parts.push_back(part);
			return parts;
		}
        parts.push_back(part);
    }

    return parts;
};

std::vector<std::string>
splitByDelim(const std::string &str, char delim)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delim))
        tokens.push_back(token);
    return tokens;
}

void
replyTo(int socket, const std::string &buffer) {
    send(socket, buffer.c_str() , buffer.size() , 0);
};

std::string
getTime()
{
    time_t clock = time(NULL);
    std::string timeFormat = ctime(&clock);
    return (timeFormat.substr(0, timeFormat.find('\n')));
};
