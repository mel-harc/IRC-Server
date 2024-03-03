/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:13:26 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:13:27 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command( const std::string& input ){
	this->input = input;
	cmds.push_back("PASS");
	cmds.push_back("NICK");
	cmds.push_back("USER");
	cmds.push_back("PRIVMSG");
	cmds.push_back("JOIN");
	cmds.push_back("MODE");
	cmds.push_back("INVITE");
	cmds.push_back("KICK");
	cmds.push_back("TOPIC");
}

bool	Command::isValidCmd (){
	std::vector<std::string>::iterator it = std::find(cmds.begin(), cmds.end(), cutCmd(this->input));
	if (it == cmds.end())
		return false;
	else
		return this->cmd = *it, true;
}

const std::string&	Command::getCmd (void ) const { return this->cmd; }

const std::string	Command::cutCmd( const std::string& input ){
	std::string		cmd;
	size_t pos = input.find(' ');
	if (pos != std::string::npos)
		cmd = input.substr(0, pos);
	else
	{
		cmd = input;
		if (cmd.rfind('\r') != std::string::npos)
			cmd.erase(cmd.rfind('\r'), 1);
		if (cmd.rfind('\n') != std::string::npos)
			cmd.erase(cmd.rfind('\n'), 1);
	}
	return cmd;
}

const std::string	Command::getNickname( std::string& input ){
	std::string		nickName = input.substr(0, 6);
	return nickName;
}

const std::string	Command::invalidCommand( const std::string& hostName, const std::string& nickName){
	std::string		replay;
	replay = ":" + hostName + " 421 " + nickName + " " + cutCmd(input) + " :Unknown command\r\n";
	return replay;
}
