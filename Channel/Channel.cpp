/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:24 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:12:25 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
	this->topic_flag = 0;
	this->inv_flag = 0;
}

Channel::Channel(std::string Name) : name(Name)
{
	this->topic_flag = 0;
	this->inv_flag = 0;
}


void Channel::setName(std::string _name)
{
	this->name = _name;
}

void Channel::setKey(std::string _key)
{
	this->key = _key;
}

bool Channel::operator==(const Channel & chan) const
{
	return (this->name == chan.name);
}

std::string Channel::getName() const
{
	return this->name;
}

std::string Channel::getKey() const
{
	return this->key;
}

void Channel::clientInsert(Client & _client)
{
	std::pair<std::string , Client> pair(_client.getNickName() ,_client);
	this->clients.insert(pair);
}

size_t Channel::size()
{
	return this->clients.size();
}

std::string& Channel::getModes()
{
	return this->modes;
}

bool Channel::isMember(std::string client_name)
{
	std::map<std::string, Client>::iterator it = clients.find(client_name);
	return (it != clients.end());
}

bool Channel::isModerator(std::string client_name)
{
	std::map<std::string, Client>::iterator it = clients.find(client_name);
	return (it->second.getMode());
}

void	Channel::addMode(std::string _mode)
{
	for (size_t i = 0; i < _mode.size() ; i++)
		if (this->modes.find(_mode[i]) > this->modes.size())
			this->modes.push_back(_mode[i]);
}

void	Channel::eraseMode(std::string _mode)
{
	size_t pos = this->modes.find(_mode);
		if ( pos <= this->modes.size())
			this->modes.erase(pos);
}

void	Channel::setLimit(int _limit)
{
	this->limit = _limit;
}

size_t Channel::getLimit()
{
	return this->limit;
}

void Channel::setTopicFlag(bool _flag)
{
	this->topic_flag = _flag;
}

void Channel::setInvFlag(bool _flag)
{
	this->inv_flag = _flag;
}

bool Channel::isInvOnly()
{
	return this->inv_flag;
}

bool Channel::isTopicOpOnly()
{
	return this->topic_flag;
}

bool Channel::isInvited(Client & _client)
{
	return (std::find(this->invList.begin(), this->invList.end(), _client.getNickName()) != this->invList.end());
}

void	Channel::BroadCast(std::map<std::string, int>& _server_clients, std::string _msg)
{
	for (std::map<std::string, Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		sendMsg(_msg, _server_clients[it->second.getNickName()]);
	}
}

void	Channel::BroadCast2(std::map<std::string, int>& _server_clients, std::string _msg,std::string _executer)
{
	for (std::map<std::string, Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (_executer != it->second.getNickName())
			sendMsg(_msg, _server_clients[it->second.getNickName()]);
	}
}

void	Channel::changeNickBroad(std::map<std::string, int>& _server_clients, std::string _msg, std::string oldNick)
{
	for (std::map<std::string, Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (it->first != oldNick)
			sendMsg(_msg, _server_clients[it->second.getNickName()]);
	}
}

void Channel::sendToClient(int _client_fd, std::string msg)
{
	sendMsg(msg, _client_fd);
}

void	Channel::eraseClient(std::string _client_Name)
{
	std::map<std::string, Client>::iterator it = this->clients.find(_client_Name);
	if (it != this->clients.end())
		this->clients.erase(it);
	std::vector<std::string>::iterator InvIt = std::find(this->invList.begin(), this->invList.end(), _client_Name);
	if (InvIt != this->invList.end())
		this->invList.erase(InvIt);
}

void	Channel::setTopic(std::string _topic)
{
	this->topic = _topic;
}

std::string Channel::getTopic()
{
	return this->topic;
}
