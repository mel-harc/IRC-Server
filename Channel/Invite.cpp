/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:41 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:12:42 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../client/Client.hpp"
#include "../server/Server.hpp"
#include "Channel.hpp"
#include "Cmd.hpp"

class InviteCmd : public Cmd{
	std::string cmd, channelName, userName;
public :
	InviteCmd(std::string str);
	std::string getChanName();
	std::string getUserNick();
};

std::string InviteCmd::getChanName()
{
	return this->channelName;
}

std::string InviteCmd::getUserNick()
{
	return this->userName;
}

InviteCmd::InviteCmd(std::string str) : Cmd(str){
	for (size_t i = 0; i < argsLen() ; i++)
		switch (i)
		{
		case 0:
			this->cmd = args[0];
			break;
		case 1:
			this->userName = args[1];
			break;
		case 2:
			this->channelName = args[2];
			break;
		default:
			break;
		}
}


int IrcServer::Invite(std::string _cmd, int id)
{
	size_t pos = _cmd.find("\r");
	if (pos <= _cmd.size())
		_cmd.erase(pos);
	pos = _cmd.find("\n");
	if (pos<=_cmd.size())
		_cmd.erase(pos);
	InviteCmd args(_cmd);
	Client executer = this->clts[this->fds[id].fd];
	if (!executer.isAuthenticate())
		return sendMsg(ErrHnadle(451, this->getHostName().c_str()), this->fds[id].fd);
	if (args.argsLen() < 3)
		return sendMsg(ErrHnadle(ERR_NEEDMOREPARAMS, args[0].c_str()), this->fds[id].fd);
	//find chan
	std::map<std::string, Channel>::iterator ChanIt = this->channels.find(args.getChanName());
	if (ChanIt == this->channels.end())
		return sendMsg(ErrHnadle(ERR_NOSUCHCHANNEL, args.getChanName().c_str()), this->fds[id].fd);
	//if executer On chan
	Channel & chan =  ChanIt->second;
	if (chan.clients.find(executer.getNickName()) == chan.clients.end())
		return sendMsg(ErrHnadle(ERR_NOTONCHANNEL, chan.getName().c_str()), this->fds[id].fd);
	executer = chan.clients[executer.getNickName()];
	//if executer is Operator
	if (!executer.getMode())
		return sendMsg(ErrHnadle(ERR_CHANOPRIVSNEEDED, chan.getName().c_str()) , this->fds[id].fd);
	//is user IN server
	std::map<std::string, int>::iterator isInserver = this->cltFds.find(args.getUserNick());
	if (isInserver == this->cltFds.end())
		return sendMsg(ErrHnadle(ERR_NOSUCHNICk, args.getUserNick().c_str()), this->fds[id].fd);
	//is user in chan
	std::map<std::string, Client>::iterator userInChanIt = chan.clients.find(args.getUserNick());
	if (userInChanIt != chan.clients.end())
		return sendMsg(ErrHnadle(441, args.getUserNick().c_str(), chan.getName().c_str()), this->fds[id].fd);
	chan.invList.push_back(args.getUserNick());
	sendMsg(("341 " +  executer.getNickName() +" " + args.getUserNick()+ " " + chan.getName()) , this->fds[id].fd);
	sendMsg((":"+executer.getNickName()+ "!~"+ executer.getUserName() +"@"+getHostName()+" INVITE " + args.getUserNick() + " " + chan.getName()), this->cltFds[args.getUserNick()]);
	return 0;
}
