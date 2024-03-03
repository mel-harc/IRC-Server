/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:44 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:29:18 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./Channel.hpp"
#include "Cmd.hpp"
#include "../server/Server.hpp"


bool CheckName(std::string str )
{
	return (str[0]=='#' && str.length( ) <= 200 );
}

bool comp(const Channel & chan1, const Channel & chan2 )
{
	return (chan1.operator==(chan2 ) );
}

std::vector<Channel> set_channels(Cmd & names, Cmd & keys )
{
	std::vector<Channel>  channels(names.argsLen( ) );
	int i =0;
	for (std::vector<std::string>::iterator it = names.args.begin( ); it != names.args.end( ); it++ )
		channels[i++].setName(*it );
	i = 0;
	for (std::vector<std::string>::iterator it = keys.args.begin( ); it != keys.args.end( ); it++ )
		channels[i++].setKey(*it );
	return channels;
}

std::string JoinErrCheck(Channel & chan, Channel & found_chan, Client & _executer )
{
	std::string msg;
	if (!found_chan.getKey().empty() && chan.getKey() != found_chan.getKey())
		return(ERR_BADCHANNELKEY(found_chan.getName()));
	if ((found_chan.isInvOnly() == 1) && !found_chan.isInvited(_executer))
	{
		return (ERR_INVONLY(found_chan.getName()));
	}
	if (found_chan.getLimit() > 0 && found_chan.size() == static_cast<size_t>(found_chan.getLimit()))
		return (ERR_CHANNELISFULL(found_chan.getName()));

	return msg;
}

void IrcServer::Join(std::string _cmd, int fd )
{
	size_t pos = _cmd.find("\r");
	if (pos <= _cmd.size())
		_cmd.erase(pos);
	pos = _cmd.find("\n");
	if (pos<=_cmd.size())
		_cmd.erase(pos);

	std::string msg(ErrHnadle(451, this->getHostName().c_str()));
	Client client = clts[fds[fd].fd];
	if (!client.isAuthenticate( ) )
	{
		sendMsg(msg, this->fds[fd].fd);
		return ;
	}
	Cmd cmd(_cmd );
	if (cmd.argsLen( ) < 2 )
	{
		msg = ERR_NEEDMOREPARAMS(client.getNickName( ) );
		send(fds[fd].fd, msg.c_str( ), msg.length( ), 0 );
		return ;
	}
	Cmd names_spl(cmd[1], ',' ), keys_spl(',' );
	if(cmd.argsLen( ) >= 3 )
		keys_spl.setCmd(cmd[2] );

	std::vector<Channel>ChannelSet = set_channels(names_spl, keys_spl );

	for (std::vector<Channel>::iterator it = ChannelSet.begin( ); it != ChannelSet.end( ); it++ )
	{
		if (!CheckName(it->getName( ) ) )
		{
			msg = ERR_NOSUCHCHANNEL(it->getName( ) );
			send(fds[fd].fd, msg.c_str( ), msg.length( ), 0 );
			continue;
		}
		_Chan::iterator found =  this->channels.find(it->getName( ) );
		if (found != this->channels.end( ) )
		{
			//see chan requirements
			if (found->second.clients.find(client.getNickName()) != found->second.clients.end())
				return ;
			msg = JoinErrCheck(*it, found->second, client );
			if (!msg.empty( ) )
				send(fds[fd].fd, msg.c_str( ), msg.length( ), 0 );
			else
			{
				if (found->second.isInvOnly())
					found->second.invList.erase(std::find(found->second.invList.begin(), found->second.invList.end(), client.getNickName()));
				client.setMode(0);
				found->second.clientInsert(client );
				std::string nameList;
				for (std::map<std::string, Client>::iterator ItOnclt = found->second.clients.begin(); ItOnclt != found->second.clients.end(); ItOnclt++)
				{
					std::string name;
					if(ItOnclt->second.getMode())
						name = "@"+ItOnclt->second.getNickName();
					else
						name = ItOnclt->second.getNickName();
					nameList += name;
					if (ItOnclt != found->second.clients.end())
						nameList += " ";
				}
				msg = ":"+ client.getNickName( )+"!"+client.getUserName() + "@"+ getHostName() + " JOIN " + it->getName( );
				found->second.BroadCast(this->cltFds, msg);
				msg = "353 " + client.getNickName() + " = " + it->getName() + " :"+nameList + "\r\n";// "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
				send(this->cltFds[client.getNickName()], msg.c_str(), msg.length(), 0);
				msg = "366 " +client.getNickName() + " " + it->getName()+" :End of /NAMES list\r\n";//  "<client> <channel> :End of /NAMES list"
				send(this->cltFds[client.getNickName()], msg.c_str(), msg.length(), 0);
			}
		}
		else
		{
			client.setMode(1);
			it->clientInsert(client);
			it->setTopicFlag(0);
			this->channels[it->getName( )] = *it;
			msg = ":"+ client.getNickName( )+"!"+client.getUserName() + "@"+ getHostName() + " JOIN " + it->getName( ) + "\r\n";
			send(fds[fd].fd, msg.c_str(), msg.length(), 0);
			msg = "353 " + client.getNickName() + " = " + it->getName() + " :"+"@"+client.getNickName() + "\r\n";// "<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}"
			send(fds[fd].fd, msg.c_str(), msg.length(), 0);
			msg = "366 " +client.getNickName() + " " + it->getName()+" :End of /NAMES list\r\n";//  "<client> <channel> :End of /NAMES list"
			send(fds[fd].fd, msg.c_str(), msg.length(), 0);
		}
	}
}
