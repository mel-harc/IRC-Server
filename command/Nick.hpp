/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 06:12:16 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/15 22:55:35 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <cctype>

#define ERR_NONICKNAMEGIVEN		":No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME	" :Erroneus nickname\r\n"
#define ERR_NICKNAMEINUSE		" :Nickname is already in use\r\n"

class Nick {
	private:
		std::string						input;
		std::string						hostName;
		std::map<std::string, int>		users;
	public:
		Nick( const std::string&, const std::string&, const std::map<std::string, int>& );
		bool				isValidParam();
		bool				isExist();
		bool				invalidChar();
		bool				validChar( char c );
		const std::string	replay( const std::string& );
		const std::string&	getHostName();
		const std::string&	getInput();
		const std::string&	getNickName();
};
