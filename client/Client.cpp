/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 22:56:03 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 16:36:55 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client (void){};
Client::Client ( int fd ){
	this->fd = fd;
	this->pw = false;
	this->existSuspendRst = false;
	this->suspendRqst = "";
}
void			Client::setRealName( const std::string& realName){ this->realName = realName; }
void			Client::setUserName( const std::string& userName ){ this->userName = userName; }
void			Client::setNickName( const std::string& nickName ){ this->nickName = nickName; }
void			Client::setSockFd( int fd ){ this->fd = fd; }
void			Client::setSuspendRequest( const std::string& Rqst ){ this->suspendRqst += Rqst; this->existSuspendRst = true; }
void			Client::setAuthenticate( int flags ){ this->pw = flags; }
bool			Client::isValidPass(){ return this->pw;}
bool			Client::isAuthenticate(){ return (realName.empty() || userName.empty() || nickName.empty() || !pw ) ? false : true;}
bool			Client::isExistSuspendRsqt() { return this->existSuspendRst;}
int				Client::getSockFd(){ return this->fd; }
const std::string&		Client::getUserName() { return this->userName;}
const std::string&		Client::getNickName() { return this->nickName; }
const std::string&		Client::getRealName() { return this->realName; }
std::string		Client::getSuspendRqst() {
	std::string		cmd(this->suspendRqst);
	this->existSuspendRst = false;
	this->suspendRqst = ""; 
	return cmd;
}

std::string		Client::notRegistred(const std::string& hostName){
	std::string	replay;
	replay = ":" + hostName + " 451 * :You have not registered\r\n"; 
	return replay;
}
