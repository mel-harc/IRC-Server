/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 19:04:19 by mel-harc          #+#    #+#             */
/*   Updated: 2024/03/03 19:09:42 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

IrcServer::~IrcServer( void ){}
int		IrcServer::getPort(){ return _port;}
IrcServer::IrcServer( const std::string& port, const std::string& passWord ){ checkArgs(port, passWord); initValue(port, passWord); }
void	IrcServer::checkArgs(const std::string& port , const std::string&passWord){ checkPort(port); checkPassWord(passWord);}
const std::string&	IrcServer::getHostName( void ) { return this->hostName; }

void	IrcServer::initValue( const std::string& port, const std::string& passWord ){
	std::stringstream ss(port);
	ss >> _port;
	_passWord = passWord;
	sockFd  = -1;
	clilen = 0;
	std::memset(&cliAddr, 0, sizeof(cliAddr));
	std::memset((char *) &servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
   	servAddr.sin_addr.s_addr = INADDR_ANY;
   	servAddr.sin_port = htons(_port);
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) != 0)
		throw std::runtime_error("Cannot get Host Name");
	this->hostName = hostname;
}

void	IrcServer::setNonBloking( void ){
	int opt = 1;

	if (setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error("Failded setsockopt");
	if (fcntl(sockFd, F_SETFL,  O_NONBLOCK) == -1)
		throw std::runtime_error("faild fcntl");
}

void	IrcServer::checkPassWord( const std::string& passWord ){
	if (passWord.empty() || passWord.find(' ') != std::string::npos)
		throw std::invalid_argument("Error: Invalid PassWord");
}

void	IrcServer::checkPort( const std::string& port ){
	if (port.empty())
		throw std::invalid_argument("Error: invalid port");
	for (size_t i = 0; i < port.size(); i++)
	{
		if (!isdigit(port[i]))
			throw std::invalid_argument("Error: invalid port");
	}
	int	p;
	std::stringstream ss(port);
	ss >> p;
	if (p >  65535 || p < 1024)
		throw std::out_of_range("Error: port out of range");
}

void	IrcServer::createSocket( void ) {
	sockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockFd < 0)
		throw std::runtime_error("Failed to create socket");
	fds.resize(1);
	fds[0].fd = sockFd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	std::cout << "\033[32m" << "opening socket successfully" << "\033[0m" << std::endl;
}

void 	IrcServer::bindSrv( void ) {
	if ( bind(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
		throw std::runtime_error("binding is Failed");
	std::cout << "\033[33m" << "Binding was work successfully" << "\033[0m" << std::endl;
}

void 	IrcServer::listningConnection() {
	int		newSockFd;
    listen(sockFd, SOMAXCONN);
    while (1)
	{
        if (poll(&fds[0], fds.size(), -1) < 0)
			throw std::runtime_error("Poll is Fialed");
		newSockFd = -1;
        for (size_t i = 0; i < fds.size(); i++)
		{
            if (fds[i].revents && POLLIN)
			{
                if (fds[i].fd == sockFd)
				{
					newSockFd = isAccept();
					pollfd 		newfd;
    				newfd.fd = newSockFd;
    				newfd.events = POLLIN;
    				fds.push_back(newfd);
					clts[newSockFd] = createClient(newSockFd);
                }
				else if (fds[i].events == POLLIN)
				{
					std::vector<std::string> cmds =  getCmds(getRequest(fds[i].fd));
					if (cmds.empty())
						removeClient(i);
					for (size_t a = 0; a < cmds.size(); a++)
					{
						if (cmds[a].length() > 510 || cmds[a].size() <= 2)
							a++;
						if (cmds[a].rfind('\n') == std::string::npos)
						{
							std::string	str = cmds[a];
							clts.at(fds[i].fd).setSuspendRequest(str);
						}
						else if (clts.at(fds[i].fd).isExistSuspendRsqt())
						{
							std::string	str = clts.at(fds[i].fd).getSuspendRqst() + cmds[a];
							execCmds(str , i);
						}
						else
							execCmds(cmds[a], i);
					}
				}
            }
        }
    }
}

std::vector<std::string>	IrcServer::getCmds(std::string input){
	std::vector<std::string>	cmds;
    size_t start = 0;
    size_t end = 0;
    while ((end = input.find("\r\n", start)) != std::string::npos)
    {
        cmds.push_back(input.substr(start, end - start + 2));
        start = end + 2;
    }
    if (start < input.size())
        cmds.push_back(input.substr(start));
    return	cmds;
}

Client	IrcServer::createClient( int fd ){
	Client	newClt(fd);
	newClt.setSockFd(fd);
	return newClt;
}

void	IrcServer::execCmds ( const std::string& input, int id ){
	Command		myCmd(input);
	if (myCmd.isValidCmd())
	{
		std::string cmd = myCmd.getCmd();
		if (cmd == "PASS")
			authenticate(input, id);
		else if (cmd == "NICK")
			setNickName(input, id);
		else if (cmd == "USER")
			setUserName(input, id);
		else if (cmd == "PRIVMSG")
			privateMsg(input, id);
		else if (cmd == "JOIN")
			Join(input, id);
		else if (cmd == "MODE")
			Mode(input, id);
		else if (cmd == "INVITE")
			Invite(input, id);
		else if (cmd == "KICK")
			Kick(input, id);
		else if (cmd == "TOPIC")
			Topic(input, id);
	}
	else if (!clts.at(fds[id].fd).isAuthenticate())
		send(fds[id].fd, clts.at(fds[id].fd).notRegistred(getHostName()).c_str(), clts.at(fds[id].fd).notRegistred(getHostName()).size(), 0);
	else
	{
		std::string	host = getHostName();
		std::string nick = clts.at(fds[id].fd).getNickName();
		send(fds[id].fd, myCmd.invalidCommand(host, nick).c_str(), myCmd.invalidCommand(host, nick).size(), 0);
	}
}


void	IrcServer::privateMsg( const std::string& input, int clientId ){
	PrivateMsg		Pmsg(input, getHostName(), clts.at(fds[clientId].fd).getNickName(), clts.at(fds[clientId].fd).getUserName(),cltFds, channels);
	if (clts.at(fds[clientId].fd).isAuthenticate())
	{
		if (Pmsg.isRecipientExist())
			send(fds[clientId].fd, Pmsg.Replay("NORECPT").c_str(), Pmsg.Replay("NORECPT").size(), 0);
		else if (Pmsg.isTextExist())
			send(fds[clientId].fd, Pmsg.Replay("NOTEXT").c_str(), Pmsg.Replay("NOTEXT").size(),0);
		else if (Pmsg.isValidCapter())
			send(fds[clientId].fd, Pmsg.Replay("CAPTER").c_str(), Pmsg.Replay("CAPTER").size(),0);
		else if (Pmsg.getCapter().find('#', 0) != std::string::npos)
		{
			if (this->channels[Pmsg.getCapter()].isMember(clts.at(fds[clientId].fd).getNickName()))
			{
				std::map<std::string, Client>::iterator it = channels.at(Pmsg.getCapter()).clients.begin();
				for (; it != channels.at(Pmsg.getCapter()).clients.end(); ++it)
					if (it->second.getSockFd() != fds[clientId].fd)
						send(it->second.getSockFd(), Pmsg.Replay("RESPONSE").c_str(), Pmsg.Replay("RESPONSE").size(), 0);
			}
		}
		else
			send(Pmsg.getFdCapter(), Pmsg.Replay("RESPONSE").c_str(), Pmsg.Replay("RESPONSE").size(), 0);
	}
	else
		send(fds[clientId].fd, clts.at(fds[clientId].fd).notRegistred(getHostName()).c_str(), clts.at(fds[clientId].fd).notRegistred(getHostName()).size(), 0);
 }

void	IrcServer::removeClient( int id ){
	this->eraseCltFromChanns(clts.at(fds[id].fd).getNickName());
	cltFds.erase(clts.at(fds[id].fd).getNickName());
	clts.erase(fds[id].fd);
	close(fds[id].fd);
	fds.erase(fds.begin() + id);
	std::cout << "\033[31m" << "Client Out" << "\033[0m" << std::endl;
}

void	IrcServer::setUserName ( std::string input, int i ){
	User	user(input, getHostName());
	if (!clts.at(fds[i].fd).isValidPass())
		send(fds[i].fd, clts.at(fds[i].fd).notRegistred(getHostName()).c_str(), clts.at(fds[i].fd).notRegistred(getHostName()).size(), 0);
	else if (clts.at(fds[i].fd).getUserName().empty())
	{
		if (user.isValidParam())
			send(fds[i].fd, user.replay("NOPARAM").c_str(), user.replay("NOPARAM").size(), 0);
		else
		{
			clts.at(fds[i].fd).setUserName(user.getUserName());
			clts.at(fds[i].fd).setRealName(user.getrealName());
			if (clts.at(fds[i].fd).isAuthenticate())
				welcoming(i);
		}
	}
	else
		send(fds[i].fd, user.replay("REGISTER").c_str(), user.replay("REGISTER").size(), 0);

}

void	IrcServer::setNickName( const std::string& nickName, int i ){
	Nick	nick(nickName, getHostName(), cltFds);
	if (clts.at(fds[i].fd).isValidPass())
	{
		if (nick.isValidParam())
			send(fds[i].fd, nick.replay("NOPARAM").c_str(), nick.replay("NOPARAM").size(), 0);
		else if (nick.invalidChar())
			send(fds[i].fd, nick.replay("CHAR").c_str(), nick.replay("CHAR").size(), 0);
		else if (nick.isExist())
			send(fds[i].fd, nick.replay("USED").c_str(), nick.replay("USED").size(), 0);
		else
		{
			std::string oldNick(this->clts[this->fds[i].fd].getNickName());
			clts.at(fds[i].fd).setNickName(nick.getNickName());
			cltFds[nick.getNickName()] = fds[i].fd;
			if (!oldNick.empty())
			{
				this->cltFds.erase(cltFds.find(oldNick));
				std::string msg(":"+ oldNick + "!~" + this->clts[this->fds[i].fd].getUserName() + "@127.0.0.1 NICK " + this->clts[this->fds[i].fd].getNickName());
				sendMsg(msg, this->fds[i].fd );
				for (std::map<std::string, Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
				{
					std::map<std::string, Client>::iterator cltIt =  it->second.clients.find(oldNick);
					if (cltIt != it->second.clients.end())
						it->second.changeNickBroad(this->cltFds, msg, oldNick);
				}
				this->setNickInChannels(oldNick, i);
			}
			else if (clts.at(fds[i].fd).isAuthenticate())
				welcoming(i);
		}
	}
	else
		send(fds[i].fd, clts.at(fds[i].fd).notRegistred(getHostName()).c_str(), clts.at(fds[i].fd).notRegistred(getHostName()).size(), 0);

}

void	IrcServer::welcoming( int id ){
	std::string nickName = clts.at(fds[id].fd).getNickName();
	std::string	replay;
	replay = ":" + getHostName() + " 001 " + nickName  + " :Welcome to the 42 Network " + nickName + "\r\n";
	send(fds[id].fd, replay.c_str(), replay.size(), 0);
}

void	IrcServer::authenticate( const std::string& pass, int i ){
	Pass	Pw(pass, _passWord, getHostName());
	if (!clts.at(fds[i].fd).isAuthenticate())
	{
		if (Pw.isValidParam())
			send(fds[i].fd, Pw.replay("NOPARAM").c_str(), Pw.replay("NOPARAM").size(), 0);
		else if (!Pw.isValidPass())
		{
			clts.at(fds[i].fd).setAuthenticate(0);
			send(fds[i].fd, Pw.replay("INVALID").c_str(), Pw.replay("INVALID").size(), 0);
		}
		else
			clts.at(fds[i].fd).setAuthenticate(1);
	}
	else
		send(fds[i].fd, Pw.replay("REREGISTER").c_str(), Pw.replay("REREGISTER").size(), 0);
}

int		IrcServer::isAccept( void ){
	clilen = sizeof(cliAddr);
    int socketfd = accept(sockFd, (struct sockaddr *)&cliAddr, &clilen);
    if (socketfd < 0)
        throw std::runtime_error("Accept Failed");
	std::cout << "\033[30m" << "Actual connection now ready"<< "\033[0m" << std::endl;
	return	socketfd;
}

std::string		IrcServer::getRequest( int newSockFd ){
	char buffer[512];
	std::memset(buffer, 0, 512);
	if (recv(newSockFd, buffer, sizeof(buffer) - 1, 0) <= 0)
		return "";
	std::string		request(buffer);
	return request;
}

void	IrcServer::eraseCltFromChanns(std::string _nickName)
{
	std::map<std::string, Channel>::iterator it = this->channels.begin();
	std::vector<std::map<std::string, Channel>::iterator> ChanEds;
	while (it != this->channels.end())
	{
		std::map<std::string, Client>::iterator cltIt = it->second.clients.find(_nickName);
		if (cltIt != it->second.clients.end())
		{
			//:KOK!~K@105.71.5.237 QUIT :Remote host closed the connection
			std::string msg = ":"+_nickName + "!~"+it->second.clients[_nickName].getUserName()+"@"+getHostName()+" QUIT :Remote host closed the connection";
			it->second.BroadCast2(this->cltFds, msg, _nickName);
			it->second.clients.erase(cltIt);
		}
		if (it->second.size() == 0)
			ChanEds.push_back(it);
		it++;
	}
	for (std::vector<std::map<std::string, Channel>::iterator>::iterator secIt = ChanEds.begin(); secIt != ChanEds.end(); secIt++)
	{
		this->channels.erase(*secIt);
	}
}

void	IrcServer::setNickInChannels(std::string _nick, int id)
{
	Client oldClt;
	for (std::map<std::string, Channel>::iterator it = this->channels.begin(); it != this->channels.end(); it++)
	{
		std::map<std::string, Client>::iterator cltIt =  it->second.clients.find(_nick);
		if (cltIt != it->second.clients.end())
		{
			oldClt = cltIt->second;
			it->second.clients.erase(cltIt);
			oldClt.setNickName(this->clts[this->fds[id].fd].getNickName());
			it->second.clients[this->clts[this->fds[id].fd].getNickName()] = oldClt;
		}
	}
}
