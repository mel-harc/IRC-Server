/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 02:28:41 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/26 23:27:22 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pass.hpp"

Pass::Pass( const std::string& input, const std::string& PassWord, const std::string& hostName ){
	this->input = input;
	this->passWord = PassWord;
	this->hostName = hostName;
	this->input.erase(0, 4);
	if (this->input.rfind('\r') != std::string::npos)
		this->input.erase(this->input.rfind('\r'), 1);
	if (this->input.rfind('\n') != std::string::npos)
		this->input.erase(this->input.rfind('\n'), 1);
	while (!this->input.empty() && this->input[0] == ' ')
    	this->input.erase(0, 1);
}

bool	Pass::isValidParam(){ return input.empty() ? true : false; }
bool	Pass::isValidPass(){ return input.compare(this->passWord) ? false : true; }
const	std::string&	Pass::getHostName() { return this->hostName; }

const std::string  Pass::replay( const std::string& hint ){
	std::string		replay;
	if (hint == "NOPARAM")
		replay = ":" + getHostName() + " 461 * PASS " + ERR_NEEDMOREPARAMSS;
	else if (hint == "INVALID")
		replay = ":" + getHostName() + " 464 * PASS " + ERR_PASSWDMISMATCH;
	else if (hint == "REREGISTER")
		replay = ":" + getHostName() + " 462 * PASS " + ERR_ALREADYREGISTERED;
	return replay;
}  


