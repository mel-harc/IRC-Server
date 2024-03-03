/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 17:42:49 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/20 20:37:38 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

#define ERR_NEEDMOREPARAMSSS 		":Not enough parameters\r\n"
#define ERR_ALREADYREGISTEREDDD		" :You are already connected and cannot handshake again\r\n"

class User {
	private:
		std::string					input;
		std::string					userName;
		std::string					realName;
		std::string					hostName;
		std::vector<std::string>	params;
	public:
		User( const std::string&, const std::string&);
		bool				isValidParam();
		const std::string&	getHostName();
		const std::string&	getUserName();
		const std::string&	getrealName();
		const std::string	replay( const std::string& );
};