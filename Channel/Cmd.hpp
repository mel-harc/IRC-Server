/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:34 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:12:35 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <vector>
#include <sstream>

class Cmd
{
	std::string					cmd;
	// size_t						param_len;
	char						delim;
public :
	std::vector<std::string>	args;
	Cmd(char _delim);
	Cmd(std::string _cmd);
	Cmd(std::string _cmd, char _delim);
	~Cmd();
	void setCmd(std::string _cmd);
	size_t argsLen();
	std::string operator[](unsigned int i);
	std::vector<std::string>& split();
};
