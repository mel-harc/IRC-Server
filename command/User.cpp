/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:42:01 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 17:49:16 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

const std::string&	User::getrealName() { return this->realName; }
const std::string&	User::getUserName() { return this->userName; }
const std::string&	User::getHostName() { return this->hostName; }
bool	User::isValidParam(){ return (params.size() < 5 || userName[0] == ':') ? true : false; }

User::User( const std::string& input, const std::string& hostName ){
	this->input = input;
	this->hostName = hostName;
	std::string					token;
	size_t						index = 0;
	if (this->input.rfind('\n') != std::string::npos)
		this->input.erase(this->input.rfind('\n'), 1);
	if (this->input.rfind('\r') != std::string::npos)
		this->input.erase(this->input.rfind('\r'), 1);
	std::string result;
  	for (size_t i = 0; i < this->input.size(); ++i)
        if (!std::isspace(this->input[i]) || (result.size() > 0 && !std::isspace(*result.rbegin())))
        	result.push_back(this->input[i]);
    if (!result.empty() && std::isspace(*result.rbegin()))
        result.erase(result.length() - 1);
	while ((index = result.find(' ')) != std::string::npos)
	{
		token = result.substr(0, index);
		params.push_back(token);
		result.erase(0, index + 1);
	}
	params.push_back(result);
	if (params.size() > 1)
		this->userName = params[1];
	if (params.size() > 3)
		this->realName = params[4];
	if (this->realName[0] == ':')
		this->realName.erase(0, 1);
}

const std::string	User::replay( const std::string& hint ){
	std::string		replay;
	if ( hint == "NOPARAM" )
		replay = ":" + getHostName() + " 461 * USER " + ERR_NEEDMOREPARAMSSS;
	else if ( hint == "REGISTER" )
		replay = ":" + getHostName() + " 462" + ERR_ALREADYREGISTEREDDD;
	return replay;
}

