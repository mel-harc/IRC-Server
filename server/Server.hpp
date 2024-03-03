/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:13:44 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:13:45 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <poll.h>
#include <fcntl.h>
#include <map>
#include <arpa/inet.h>
#include "../client/Client.hpp"
#include "../command/Command.hpp"
#include "../command/PrivateMsg.hpp"
#include "../command/Pass.hpp"
#include "../command/Nick.hpp"
#include "../command/User.hpp"
#include "../Channel/Channel.hpp"
#include "../bot/Bot.hpp"

class Channel;
typedef std::map<std::string, Channel> _Chan;
class IrcServer {
	private:
		int 									sockFd;
		socklen_t 								clilen;
		struct sockaddr_in						servAddr;
		int										_port;
		std::string								_passWord;
		std::string								hostName;
		std::map<int, Client>					clts;
		std::map<std::string, int>				cltFds;
		std::vector<pollfd>						fds;
	public:
		struct sockaddr_in						cliAddr;
		IrcServer( const std::string&, const std::string&);
		~IrcServer( void );
		void				checkArgs(const std::string&, const std::string&);
		void				checkPort( const std::string&);
		void				checkPassWord( const std::string&);
		void				initValue( const std::string&, const std::string&);
		void				createSocket( void );
		void				bindSrv( void );
		void				listningConnection( void );
		const std::string&	getHostName( void );
		std::string			getRequest( int newSockFd );
		void				setResponse( int fd, const std::string& msg );
		void				welcoming(int id);
		int					getPort();
		int					isAccept( void );
		void				authenticate( const std::string& pass, int fd );
		void				execCmds( const std::string& , int fd );
		std::vector<std::string>	getCmds(std::string input);
		Client				createClient( int fd );
		void				setNickName( const std::string& nick, int fd );
		void				setUserName( std::string nick, int fd );
		void				setNonBloking( void );
		void				removeClient( int id );
		void				privateMsg( const std::string& input, int clientId );
		//
		void				Join(std::string cmd, int id);
		std::map<std::string, Channel>			channels;
		int					Mode(std::string _cmd, int id);
		int					Invite(std::string _cmd, int id);
		void				Kick(std::string _cmd, int id);
		int					Topic(std::string _cmd, int id);
		void				eraseCltFromChanns(std::string _nickName);
		void				setNickInChannels(std::string _nick, int id);
};
