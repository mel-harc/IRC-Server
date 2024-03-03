/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 02:28:38 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/17 20:36:01 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

#define ERR_NEEDMOREPARAMSS		":Not enough parameters\r\n"
#define ERR_PASSWDMISMATCH		":Password incorrect\r\n"
#define ERR_ALREADYREGISTERED 	":You may not reregister\r\n"

class Pass {
	private:
		std::string		input;
		std::string		passWord;
		std::string		hostName;
	public:
		Pass(const std::string&, const std::string&, const std::string& );
		bool					isValidParam();
		bool					isValidPass();
		const std::string		replay( const std::string& hinst );
		const std::string&		getHostName();	
};