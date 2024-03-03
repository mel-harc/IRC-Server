/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 22:09:26 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 16:37:15 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PrivateMsg.hpp"

PrivateMsg::PrivateMsg (const std::string& input, const std::string& hostName, const std::string& nickName, const std::string& userName, std::map<std::string, int>& clts, std::map<std::string,  Channel>& channels){
	this->input = input;
	this->channels = channels;
	this->hostName = hostName;
	this->nickName = nickName;
	this->userName = userName;
	this->clts = clts;
	if (this->input.rfind('\n') != std::string::npos)
		this->input.erase(this->input.rfind('\n'), 1);
	if (this->input.find('\r') != std::string::npos)
		this->input.erase(this->input.find('\r'), 1);
	if (this->input.find("PRIVMSG ", 1))
		this->input.erase(0, 8);
	while (!this->input.empty() && this->input[0] == ' ')
    	this->input.erase(0, 1);
	std::stringstream 	ss(this->input);
	if (std::getline(ss, capter, ' '))
		this->input.erase(0, capter.length() + 1);
	while (!this->input.empty() && this->input[0] == ' ')
    	this->input.erase(0, 1);
	if (this->input.find(':', 0) == std::string::npos)
	{
		std::getline(ss, this->msg, ' ');
		this->msg.insert(this->msg.begin(), ':');
		this->msg += "\r\n";
	}
	else
		this->msg = this->input + "\r\n";
}
const std::string&	PrivateMsg::getHostName () { return this->hostName; }
const std::string&	PrivateMsg::getNickName () { return this->nickName; }
const std::string&	PrivateMsg::getUserName () { return this->userName; }
const std::string&	PrivateMsg::getCapter () { return this->capter; }
const std::string&	PrivateMsg::getMsg () { return this->msg; }
bool	PrivateMsg::isRecipientExist ( void ){ return capter.empty() ? true : false; }
bool	PrivateMsg::isCapterExist() { return capter.empty() ? true : false; }
bool	PrivateMsg::isTextExist() { return (msg.empty() || (msg.size() < 4 && msg[0] == ':' )) ? true : false; }
int		PrivateMsg::getFdCapter() { return clts.at(capter); }
std::string		PrivateMsg::Replay( const std::string& hint ){
	std::string		replay;
	if (hint == "RESPONSE")
		replay = ":" + getNickName() + "!" + getUserName() + "@" + getHostName() + " PRIVMSG " + getCapter() + " " + getMsg();
	else if (hint == "NORECPT")
		replay = ":" + getHostName() + " 411 " + getNickName() + ERR_NORECIPIENT + "\r\n";
	else if (hint == "NOTEXT")
		replay = ":" + getHostName() + " 412 " + getNickName() + ERR_NOTEXTTOSEND + "\r\n";
	else if (hint == "CAPTER")
		replay = ":" + getHostName() + " 401 " + getNickName() + ERR_NOSUCHNICK + "\r\n";
	return replay;
}

bool	PrivateMsg::isValidCapter() {
	if (capter.find('#', 0) != std::string::npos)
	{
		std::map<std::string, Channel>::iterator it = channels.find(capter);
		return it == channels.end();
	}
	else
	{
		std::map<std::string, int>::iterator it = clts.find(capter);
		return it == clts.end();
	}
}
