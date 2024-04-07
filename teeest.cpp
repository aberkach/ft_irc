/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   teeest.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abberkac <abberkac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 01:12:23 by abberkac          #+#    #+#             */
/*   Updated: 2024/04/06 01:17:21 by abberkac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

struct JoinCommand {
    std::vector<std::string> channels;
    std::vector<std::string> keys;
};

JoinCommand parseJoinCommand(const std::string& command) {
    JoinCommand joinCmd;
    std::istringstream iss(command);
    std::string token;
    iss >> token; // skip the "JOIN" keyword
    while (iss >> std::ws) { // Skip whitespace
        if (iss.peek() == ',') { // If there's a comma
            iss.ignore(); // Ignore the comma
        }
        std::getline(iss, token, ','); // Read the next token separated by comma
        if (token.empty()) // Skip empty tokens
            continue;
        std::istringstream channelAndKey(token);
        std::string channel, key;
        std::getline(channelAndKey, channel, ' '); // Extract channel name
        std::getline(channelAndKey, key, ' '); // Extract key
        joinCmd.channels.push_back(channel);
        joinCmd.keys.push_back(key);
    }
    return joinCmd;
}

int main() {
    // Example JOIN command
    std::string joinCommand = "JOIN #foo,#bar,         sd fubar,foobar,     as";

    // Parsing the JOIN command
    JoinCommand joinCmd = parseJoinCommand(joinCommand);

    // Displaying parsed data
    std::cout << "Channels: ";
    for (const auto& channel : joinCmd.channels) {
        std::cout << channel << " ";
    }
    std::cout << std::endl;

    std::cout << "Keys: ";
    for (const auto& key : joinCmd.keys) {
        std::cout << key << " ";
    }
    std::cout << std::endl;

    return 0;
}
