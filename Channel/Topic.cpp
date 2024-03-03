/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:13:07 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:13:08 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "../server/Server.hpp"
#include "topicCmd.hpp"

int IrcServer::Topic(std::string _inputCmd, int id)
{
	std::string InputCmd(_inputCmd);
	size_t pos = InputCmd.find("\r");
	if (pos <= InputCmd.size())
		InputCmd.erase(pos);
	pos = InputCmd.find("\n");
	if (pos<=InputCmd.size())
		InputCmd.erase(pos);

	Client & executer = this->clts[this->fds[id].fd];
	if (!executer.isAuthenticate())
		return sendMsg(ErrHnadle(451, getHostName().c_str()), this->fds[id].fd);

	topicCmd parsed(InputCmd);
	if (parsed.argsLen() < 2)
		return sendMsg(ErrHnadle(ERR_NEEDMOREPARAMS, parsed[0].c_str()), this->fds[id].fd);
	std::map<std::string, Channel>::iterator ChanIt = this->channels.find(parsed.getChannelName());
	if (ChanIt == this->channels.end())
		return sendMsg(ErrHnadle(ERR_NOSUCHCHANNEL, parsed.getChannelName().c_str()), this->fds[id].fd);
	Channel & chan = ChanIt->second;
	std::string executerName = this->clts[this->fds[id].fd].getNickName();
	if (parsed.argsLen() == 2)
	{
		std::string top = chan.getTopic();
		if (top.empty())
			sendMsg(ErrHnadle(331, executerName.c_str(),chan.getName().c_str()), this->fds[id].fd);
		else
			sendMsg(ErrHnadle(332, executerName.c_str(), chan.getName().c_str(), top.c_str()), this->fds[id].fd);
		return 0;
	}

	bool isInChan = (chan.clients.find(executerName) != chan.clients.end());
	if (parsed.argsLen() > 2 && !isInChan)
		return sendMsg(ErrHnadle(ERR_NOTONCHANNEL, chan.getName().c_str()), this->fds[id].fd);
	if (chan.isTopicOpOnly() &&parsed.argsLen() > 2 && !chan.clients[executerName].getMode())
		return sendMsg(ErrHnadle(ERR_CHANOPRIVSNEEDED, chan.getName().c_str()), this->fds[id].fd);

	chan.setTopic(parsed.getTopic());
	chan.BroadCast(this->cltFds, (":" + executerName+ "!~" + chan.clients[executerName].getUserName() +"@127.0.0.1 TOPIC "+ chan.getName() + " " + chan.getTopic()));
	return 0;
}
