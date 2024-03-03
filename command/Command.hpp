/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 16:05:48 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/20 21:22:21 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

class Command {
	private:
		std::vector<std::string>	cmds;
		std::string					cmd;
		std::string					input;
	public:
		Command( const  std::string& input );
		const std::string	cutCmd( const std::string& input );
		bool				isValidCmd(  );
		const std::string&	getCmd( void ) const;
		const std::string	getNickname( std::string& input );
		const std::string	invalidCommand( const std::string&, const std::string&);
};