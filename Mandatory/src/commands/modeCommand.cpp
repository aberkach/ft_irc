
#include "../../Inc/irc.hpp"

#define __KNOWNMODECHAR(charMode) ( 				   \
					    			charMode == 'i' || \
					    			charMode == 't' || \
					    			charMode == 'k' || \
					    			charMode == 'o' || \
					    			charMode == 'l'    \
								  )

void
Server::modeSetReply(Client& client, Channel &channel, std::string& modes, const std::vector<std::string> &fields)
{
	if (modes.empty())
		return;

	for (size_t i = 0; i < fields.size() ; i++)
		modes += ' ' + fields[i];

	channel.broadCast(MODE_SET(client.getNickname(), client.getUsername(), inet_ntoa(client.getAddr().sin_addr), channel.getName(), modes), -1);
};

void
Server::displayChannelMode(const Channel &channel, const Client &client)
{
	std::string params = "+";
	std::string applied = " ";

	if (channel.getTopicFlag())
		params += "t";
	if (channel.getIsInviteOnly())
		params += "i";
	if (channel.getMaxUsers() != 0) {
		params += "l";
		applied += to_string(channel.getMaxUsers()) + " ";
	}
	if (!channel.getKey().empty()) {
		params += "k";
		applied += channel.getKey();
	}

	replyTo(client.getSocket(), RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), (params + applied)));
	replyTo(client.getSocket(), RPL_CREATIONTIME(client.getNickname(), channel.getName(), channel.getCreationTime()));
};

std::string
Server::extractModeString(const std::string &modeField, Client &client)
{
    std::set<char> modes;
    std::string modesString;
    char sign = (modeField[0] == '-') ? '-' : '+';

    for (size_t i = 0; i < modeField.size(); i++)
	{
        while (modeField[i] && modeField[i] == sign)
            i++;
        if (modeField[i] != sign && (modeField[i] == '-' || modeField[i] == '+'))
            sign = modeField[i];
        else if __KNOWNMODECHAR(modeField[i])
		{
			if (modes.find(modeField[i]) == modes.end() || modeField[i] == 'o')
			{
                modesString += sign;
                modesString += modeField[i];
				modes.insert(modeField[i]);
			}
        }
		else if (modeField[i] != '-' && modeField[i] != '+')
            replyTo(client.getSocket(), ERR_UNKNOWNMODE(client.getNickname(), modeField[i]));
    }

    return modesString;
};

static void
checksign(char &appliedSign, char sign, std::string& appliedModes, char mode)
{
	if (appliedSign != sign)
	{
		appliedSign = sign;
		appliedModes += appliedSign;
	}
	appliedModes += mode;
};

void
Server::executeModes(const std::vector<std::string> &fields, Client &client, chanIt it)
{
	size_t arg = 2;
	char sign = '\0';
	std::string	appliedModes;
	std::vector<std::string> appliedFields;
	std::string modestr = extractModeString(fields[1], client);

	for (size_t i = 0; i < modestr.size(); i += 2)
	{
		bool set = (modestr[i] == '+');

		switch (modestr[i + 1])
		{
			case 'i':
				checksign(sign, modestr[i], appliedModes, 'i');
				it->second.setIsInviteOnly(set);
				break;
			case 't':
				checksign(sign, modestr[i], appliedModes, 't');
				it->second.setTopicFlag(set);
				break;
			case 'k':
				if (set && arg < fields.size())
				{
					it->second.setKey(fields[arg]);
					checksign(sign, modestr[i], appliedModes, 'k');
					appliedFields.push_back(fields[arg]);
					arg++;
				}
				else if (!set)
					it->second.setKey("");
				break;
			case 'o':
				if (arg < fields.size())
				{
					if (it->second.isClientExist(fields[arg]))
					{
						if (set)
							it->second.addOperator(it->second.getUser(fields[arg]));
						else
							it->second.removeOperator(it->second.getUser(fields[arg]));
						checksign(sign, modestr[i], appliedModes, 'o');
						appliedFields.push_back(fields[arg]);
					}
					else
						replyTo(client.getSocket(), ERR_NOTONCHANNEL(client.getNickname(), it->second.getName()));
					arg++;
				}
				break;
			case 'l':
				if (set && arg < fields.size())
				{
					long maxUsers = std::atol(fields[arg].c_str());
					if (maxUsers > 0 && fields.size() <= 10 && maxUsers <= std::numeric_limits<int>::max())
					{
						it->second.setMaxUsers(maxUsers);
						checksign(sign, modestr[i], appliedModes, 'l');
						appliedFields.push_back(fields[arg]);
					}
					arg++;
				}
				else if (!set) {
					checksign(sign, modestr[i], appliedModes, 'l');
					it->second.setMaxUsers(0);
				}
				else
					replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "+l"));
				break;
			default:
				break;
		}

	}
	modeSetReply(client, it->second, appliedModes, appliedFields);
};

void
Server::modeCommand(const std::vector<std::string> &fields, Client &client)
{
	if (client.getRegistered())
	{
		if (fields.empty())
		{
			replyTo(client.getSocket(), ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));
			return;
		}
		chanIt it = _channels.find(fields[0]);
		if (it != _channels.end())
		{
			int size = fields.size();
			switch (size)
			{
				case 1:
					displayChannelMode(it->second, client);
					break;

				default:
					if (it->second.isOperator(client.getNickname()))
						executeModes(fields, client, it);
					else
						replyTo(client.getSocket(), ERR_CHANOPRIVSNEEDED(client.getNickname(), fields[0]));
					break;
			}
		}
		else
			replyTo(client.getSocket(), ERR_NOSUCHCHANNEL(client.getNickname(), "MODE"));
	}
	else
        replyTo(client.getSocket(), ERR_NOTREGISTERED(std::string("GUEST")));
};
