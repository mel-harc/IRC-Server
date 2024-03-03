/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCmd.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:54 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:12:55 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kickCmd.hpp"

void	kickCmd::setNames(std::string str)
{
	Cmd cmd(str, ',');
	this->names = cmd.args;
}

kickCmd::kickCmd(std::string str) : Cmd(str)
{
	this->channel = args[1];
	setNames(args[2]);
	setComment();
}

void	kickCmd::setComment()
{
	if (this->argsLen() < 4)
		return ;
	if (args[3][0] != ':')
	{
		comment = args[3];
		return ;
	}
	else
		args[3].erase(0,1);
	for (size_t i = 3; i < argsLen(); i++)
	{
		comment.append(args[i]);
		if (i + 1 < argsLen())
			comment.append(" ");
	}
}

size_t kickCmd::kickLen()
{
	size_t count = 0;
	for (size_t i = 0 ; i < argsLen(); i++)
		if (!args[i].empty())
			count++;
	return count;
}
