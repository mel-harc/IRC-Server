/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:13:00 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:13:01 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../client/Client.hpp"
#include "Channel.hpp"
#include <stdarg.h>

class Token
{
public :
	char sign;
	char flag;
	std::string param;
	Token(char _sign, char _flag, std::string _param);
};

Token::Token(char _sign, char _flag, std::string _param) : sign(_sign), flag(_flag), param(_param)
{
}

class modeCmd: public Cmd
{
private:
public :
	std::string cmd;
	std::string channel;
	std::string modes;
	std::vector<std::string> params;
	modeCmd(std::string _cmd);
};

modeCmd::modeCmd(std::string _cmd) : Cmd(_cmd)
{
	for (size_t i = 0; i < argsLen(); i++)
	{
		switch (i)
		{
		case 0:
			cmd = args[i];
			break;
		case 1:
			channel = args[i];
			break;
		case 2:
			modes = args[i];
			break;
		default:
			params.push_back(args[i]);
			break;
		}
	}
}

int sendMsg(std::string str, int _fd)
{
	str += "\r\n";
	send(_fd, str.c_str(), str.length(), 0);
	return 1;
}

bool Client::getMode()
{
	return this->isMode;
}

void Client::setMode(bool _mode)
{

	this->isMode = _mode;
}

std::map<std::string , Channel>::iterator getChan(std::map<std::string , Channel> & _channels, std::string _name)
{
	return (_channels.find(_name));
}

std::map<std::string, Client>::iterator getChannelClient(std::map<std::string, Client> & _clients, std::string name)
{
	return (_clients.find(name));
}

int	IrcServer::Mode(std::string _cmd, int id)
{
	size_t pos = _cmd.rfind("\r");
	if (pos <= _cmd.size())
		_cmd.erase(pos);
	pos = _cmd.rfind('\n');
	if (pos <= _cmd.size())
		_cmd.erase(pos);

	modeCmd parsed(_cmd);
	Client & executer = this->clts[this->fds[id].fd];
	if (!executer.isAuthenticate())
		return sendMsg(ErrHnadle(451, getHostName().c_str()), this->fds[id].fd);
	if (parsed.argsLen() < 2)
		return sendMsg(ErrHnadle(ERR_NEEDMOREPARAMS, parsed.channel.c_str()), this->fds[id].fd);
	std::map<std::string, Channel>::iterator chanIt = getChan(this->channels, parsed.channel);
	if (chanIt == this->channels.end())
		return sendMsg(ErrHnadle(ERR_NOSUCHCHANNEL, parsed.channel.c_str()), this->fds[id].fd); // Cahnnel not foun

	if (parsed.argsLen() == 2)
	{
		sendMsg(ErrHnadle(324, executer.getNickName().c_str(), chanIt->second.getName().c_str(), chanIt->second.getModes().c_str()), this->fds[id].fd);
		return(0);
	}

	if (chanIt->second.clients.find(executer.getNickName()) == chanIt->second.clients.end())
		return sendMsg(ErrHnadle(ERR_NOTONCHANNEL, parsed.channel.c_str()), this->fds[id].fd); // YOUR not on this channel;

	if (!chanIt->second.clients[executer.getNickName()].getMode() && parsed.argsLen() > 2)
		return sendMsg(ErrHnadle(ERR_CHANOPRIVSNEEDED, parsed.channel.c_str()), this->fds[id].fd);// You r not Channel OP

	std::string sign =  "+";
	std::vector<Token> tokens;
	std::vector<std::string>::iterator paramsIt = parsed.params.begin();


	for(int i = 0 ; parsed.modes[i] ; i++)
	{
		switch (parsed.modes[i])
		{
		case 'i':
			// setInv(chan);
			if (sign == "+" && !chanIt->second.isInvOnly())
			{
				tokens.push_back(Token(sign[0], parsed.modes[i], ""));
				chanIt->second.setInvFlag(1);
				chanIt->second.addMode("i");
			}
			else if (sign == "-" && chanIt->second.isInvOnly())
			{
				tokens.push_back(Token(sign[0], parsed.modes[i], ""));
				chanIt->second.setInvFlag(0);
				chanIt->second.eraseMode("i");
				chanIt->second.invList.clear();
			}
			break;
		case 't':
				if (sign == "+" && !chanIt->second.isTopicOpOnly())
				{
					tokens.push_back(Token(sign[0], parsed.modes[i], ""));
					chanIt->second.setTopicFlag(1);
					chanIt->second.addMode("t");
				}
				else if (sign == "-" && chanIt->second.isTopicOpOnly())
				{
					tokens.push_back(Token(sign[0], parsed.modes[i], ""));
					chanIt->second.setTopicFlag(0);
					chanIt->second.eraseMode("t");
				}
			break;
		case 'k':
			if (sign == "+" && (paramsIt == parsed.params.end()) )
				break;
			if (sign == "+")
			{
				//setKey
				chanIt->second.setKey(*(paramsIt));
				tokens.push_back(Token(sign[0], 'k', *(paramsIt++)));
				chanIt->second.addMode("k");
			}
			else
			{
				//unsetKey
				chanIt->second.setKey("");
				tokens.push_back(Token(sign[0], 'k', std::string("")));
				chanIt->second.eraseMode("k");
			}
			break;
		case 'o':
			if ((paramsIt == parsed.params.end()))
				break ;
			else
			{
				std::map<std::string, Client>::iterator userIt = getChannelClient(chanIt->second.clients, *paramsIt);
				std::map<std::string, int>::iterator cltIt = this->cltFds.find(*paramsIt);
				if (cltIt == this->cltFds.end())
				{
					sendMsg(ErrHnadle(ERR_NOSUCHNICk, (*paramsIt).c_str()), this->fds[id].fd);// You r not Channel OP
					paramsIt++;
				}
				else if (userIt == chanIt->second.clients.end())
				{
					sendMsg(ErrHnadle(ERR_USERNOTINCHANNEL, (*paramsIt).c_str() ,parsed.channel.c_str()), this->fds[id].fd);// You r not Channel OP
					paramsIt++;
				}
				else
				{
					if (sign == "+" && !userIt->second.getMode())
					{
						tokens.push_back(Token(sign[0] ,'o' ,*(paramsIt++)));
						userIt->second.setMode(1);
					}
					else if (sign == "-" && userIt->second.getMode() && (executer.getNickName() != userIt->second.getNickName()))
					{
						userIt->second.setMode(0);
						tokens.push_back(Token(sign[0] ,'o' ,*(paramsIt++)));
					}
				}
			}
			break;
		case 'l':
			if (sign == "+" && (paramsIt == parsed.params.end()))
				break;
			if (sign == "+" && !atoi((*paramsIt).c_str()))
			{
				paramsIt++;
				break;
			}
			else if (sign == "+" && atoi((*paramsIt).c_str()))
			{
				chanIt->second.setLimit(atoi((*paramsIt).c_str()));
				tokens.push_back(Token(sign[0], 'l', *paramsIt));
				chanIt->second.addMode("l");
				paramsIt++;
			}
			else
			{
				chanIt->second.setLimit(0);
				tokens.push_back(Token(sign[0], 'l', ""));
				chanIt->second.eraseMode("l");
			}
			break;
		case '+':
			sign= "+";
			break;
		case '-':
			sign = "-";
			break;
		case 's' :
			break;
		case 'n' :
			break;
		default :
			sendMsg(ErrHnadle(472, executer.getNickName().c_str() ,&parsed.modes[i]), this->fds[id].fd);
			break;
		}
	}

	std::vector<Token>::iterator prev = tokens.begin();
	std::string replay;
	for (std::vector<Token>::iterator it = tokens.begin(); it != tokens.end() ; it++)
	{
		if (it->sign != prev->sign || it == tokens.begin())
		{
			replay.push_back(it->sign);
			prev->sign = it->sign;
		}
		replay.push_back(it->flag);
	}
	replay.append(" ");
	for (std::vector<Token>::iterator it = tokens.begin(); it != tokens.end() ; it++)
	{
		if (!it->param.empty())
			replay.append(it->param);
		if (it != tokens.end())
			replay.append(" ");
	}

	if (tokens.size() > 0)
		chanIt->second.BroadCast(this->cltFds, "MODE " + parsed.channel +" " + replay.c_str());
	return 0;
}
