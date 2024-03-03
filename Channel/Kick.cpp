#include "../server/Server.hpp"
#include "../client/Client.hpp"
#include "Cmd.hpp"
#include "Channel.hpp"
#include "kickCmd.hpp"

static int sendMsgJoin(std::string str, int _fd)
{
	send(_fd, str.c_str(), str.length(), 0);
	return 1;
}


std::map<std::string, Channel>::iterator getChannel(std::map<std::string, Channel> & _channels, std::string name)
{
	std::map<std::string, Channel>::iterator ChanIt = _channels.find(name);
	return ChanIt;
}

std::map<std::string, Client>::iterator getUser(std::map<std::string, Client> & _clients, std::string name)
{
	std::map<std::string, Client>::iterator clientItOnChan = _clients.find(name);
	return clientItOnChan;
}

bool	isInServe(std::map<std::string, int> _clients, std::string _name)
{
	return (_clients.find(_name) != _clients.end());
}

void	IrcServer::Kick(std::string _cmd, int id)
{
	std::string msg;
	size_t pos = _cmd.rfind("\r");
	if (pos <= _cmd.size())
		_cmd.erase(pos);
	pos = _cmd.rfind("\n");
	if (pos<=_cmd.size())
		_cmd.erase(pos);

	Client executer = this->clts[this->fds[id].fd];
	std::string ip_s(getHostName());
	if (!executer.isAuthenticate())
	{
		msg = ErrHnadle(451, getHostName().c_str());
		sendMsg(msg, this->fds[id].fd);
		return ;
	}
	kickCmd cmd(_cmd);
	if (cmd.kickLen() < 3)
	{
		sendMsg(ErrHnadle(ERR_NEEDMOREPARAMS, cmd[0].c_str()), this->fds[id].fd);
		return ;
	}

	std::map<std::string, Channel>::iterator ChanIt = getChannel(this->channels, cmd.channel);
	if (ChanIt == this->channels.end())
	{
		msg = ErrHnadle(ERR_NOSUCHCHANNEL, cmd.channel.c_str());
		sendMsg(msg, this->fds[id].fd);
		return ;
	}
	Channel & chan = ChanIt->second;
	if (!chan.isMember(executer.getNickName()))
	{
		msg = ":"+ip_s+ " 424 " + executer.getNickName() + " " + cmd.channel + " :You're not on that channel\r\n";
		sendMsgJoin(msg, this->cltFds[executer.getNickName()]);
		return;
	}
	executer = chan.clients[executer.getNickName()];
	if (!executer.getMode())
	{
		msg = ":"+ ip_s + " 482 " + executer.getNickName() + " " + cmd.channel +  " :You're not a channel operator\r\n";
		sendMsgJoin(msg, this->cltFds[executer.getNickName()]); //:osmium.libera.chat 482 OP #CHAN1 :You're not a channel operator
		return;
	}
	for (std::vector<std::string>::iterator namesIt = cmd.names.begin(); namesIt != cmd.names.end(); namesIt++)
	{
		if (!isInServe(this->cltFds, *namesIt))
		{
			msg = ":" + ip_s + " 401 " + executer.getNickName() + " " + *namesIt + " :No such nick/channel\r\n";
			sendMsgJoin(msg, this->cltFds[executer.getNickName()]);
			continue;
		}
		std::map<std::string, Client>::iterator user = getUser(chan.clients, *namesIt);
		if (user == chan.clients.end())
		{
			msg = ":"+ip_s+" 441 " + executer.getNickName() + " " + user->first + " " + cmd.channel + " :They aren't on that channel\r\n";
			sendMsgJoin(msg, this->cltFds[executer.getNickName()]);
			continue;
		}
		msg = ":"+executer.getNickName()+"!"+executer.getUserName()+"@"+ip_s + " KICK " + cmd.channel + " "+ user->first ;  //:WiZ!jto@tolsun.oulu.fi KICK #Finnish John
		if (!cmd.comment.empty())
			msg.append(" :" + cmd.comment);
		msg.append("\r\n");
		for (std::map<std::string,Client>::iterator it = chan.clients.begin(); it != chan.clients.end(); it++)
			sendMsgJoin(msg, this->cltFds[it->first]);
		chan.clients.erase(chan.clients.find(*namesIt));
	}
}
