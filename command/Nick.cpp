/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 06:12:18 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 15:21:53 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"

Nick::Nick( const std::string& input, const std::string& hostName, const std::map<std::string, int>& clts ) {
	this->input = input;
	this->users = clts;
	this->hostName = hostName;
	this->input.erase(0, 4);
	if (this->input.rfind('\r') != std::string::npos)
		this->input.erase(this->input.rfind('\r'), 1);
	if (this->input.rfind('\n') != std::string::npos)
		this->input.erase(this->input.rfind('\n'), 1);
	while (!this->input.empty() && this->input[0] == ' ')
    	this->input.erase(0, 1);
}

const std::string&	Nick::getHostName(){ return this->hostName; }
const std::string&	Nick::getNickName(){ return this->input;}
const std::string&	Nick::getInput(){ return this->input; }
bool	Nick::isValidParam() { return (input.empty() || input.size() > 9 || (input[0] == ':' && input.size() == 1)) ? true : false; }
bool	Nick::validChar( char c ) {
	if (c != '[' && c != ']' && c != '{' && c != '}' && c != '|' && c != '_')
		return true;
	return false;
}

bool	Nick::isExist() {
	std::map<std::string, int>::iterator i;
	std::map<std::string, int>::iterator it = users.find(input);
	return it != users.end();
}

bool	Nick::invalidChar(){
	if (isdigit(input[0]))
		return true;
	for (size_t i = 0; i < input.size(); i++)
	{
		if (!std::isalnum(input[i]) && !isdigit(input[i]) && validChar(input[i]))
			return true;
	}
	return false;
}

const std::string	Nick::replay( const std::string& hint ){
	std::string		replay;
	if (hint == "NOPARAM")
		replay = ":" + getHostName() + " 431 * " + ERR_NONICKNAMEGIVEN;
	else if (hint == "USED")
		replay = ":" + getHostName() + " 433 * " + getInput() + ERR_NICKNAMEINUSE;
	else if (hint == "CHAR")
		replay = ":" + getHostName() + " 432 * " + getInput() + ERR_ERRONEUSNICKNAME;
	return	replay;
}

