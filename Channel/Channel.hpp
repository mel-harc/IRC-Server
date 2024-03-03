/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:27 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 19:00:37 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <cstdarg>
#include <map>
#include "../server/Server.hpp"
#include "../client/Client.hpp"
#include "Cmd.hpp"

#define ERR_NEEDMOREPARAMS(x ) "461 " + x +  " JOIN:Not enough parameters\r\n"
#define ERR_NOSUCHCHANNEL(x ) "403 " + x + " :No such channel\r\n"
#define ERR_BADCHANNELKEY(x ) "475 " + x + " :Cannot join channel (+k)\r\n"
#define ERR_CHANNELISFULL(x ) "471 " + x + " :Cannot join channel (+l)\r\n"
#define ERR_INVONLY(X) "473 " + X + " :Cannot join channel (+i)\r\n"

enum Err{
	ERR_NEEDMOREPARAMS = 461,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_NOTONCHANNEL = 442,
	ERR_UNKNOWNMODE,
	RPL_CHANNELMODEIS = 324,
	ERR_NOSUCHNICk = 401,
	ERR_KEYSET = 467,
	ERR_USERSDONTMATCH = 502,
	ERR_UMODEUNKNOWNFLAG = 501,
	RPL_UMODEIS,
	ERR_USERNOTINCHANNEL = 441
};

class Channel
{
	std::string						name;
	std::string						key;
	std::string						modes;
	std::string						topic;
	size_t							limit;
	bool							topic_flag;
	bool							inv_flag;

public:
	Channel();
	Channel(std::string Name);
	std::map<std::string, Client>	clients;
	void setName(std::string _name);
	std::string						getName() const;
	std::string						getKey() const;
	void							setKey(std::string _key);
	bool							operator==(const Channel & chan) const;
	void							clientInsert(Client & _client);
	size_t							size();
	std::string						&getModes();
	void							addMode(std::string _mode);
	void							eraseMode(std::string _mode);
	bool							isMember(std::string client_name);
	bool							isModerator(std::string client_name);
	void							setLimit(int _limit);
	size_t							getLimit();
	bool							isInvited(Client & _client);
	std::vector<std::string>		invList;
	void							setTopicFlag(bool _flag);
	void							setTopic(std::string _topic);
	std::string						getTopic();
	void							setInvFlag(bool _flag);
	bool							isInvOnly();
	bool							isTopicOpOnly();
	void							BroadCast(std::map<std::string, int> & _server_clients, std::string _msg);
	void							sendToClient(int _client_fd, std::string msg);
	void							eraseClient(std::string _clien_Name);
	void							changeNickBroad(std::map<std::string, int>& _server_clients, std::string _msg, std::string oldNick);
	void							BroadCast2(std::map<std::string, int>& _server_clients, std::string _msg, std::string _executer);
};

typedef std::map<std::string, Channel> _Chan;
std::string ErrHnadle(int Error, ...);
int sendMsg(std::string str, int _fd);
