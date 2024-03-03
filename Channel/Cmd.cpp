/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:31 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:12:32 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

std::vector<std::string>& Cmd::split()
{
	std::stringstream ss(this->cmd);
	std::string token;

	while (getline(ss, token, this->delim))
		args.push_back(token);
	return args;
}

Cmd::Cmd(char _delim) : delim(_delim)
{
}

void Cmd::setCmd(std::string _cmd)
{
	this->cmd = _cmd;
	this->split();
}

Cmd::Cmd(std::string _cmd) : cmd(_cmd), delim(' ')
{
	split();
}

Cmd::Cmd(std::string _cmd, char _delim) : cmd(_cmd), delim(_delim)
{
	split();
}

size_t Cmd::argsLen()
{
	return args.size();
}

Cmd::~Cmd()
{
	this->args.clear();
}

std::string Cmd::operator[](unsigned int i)
{
	return this->args[i];
}
