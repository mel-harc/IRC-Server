/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 22:04:47 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/23 01:46:53 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include "../Channel/Channel.hpp"

#define 	ERR_NOTEXTTOSEND	" :No text to send"
#define 	ERR_NOSUCHNICK		" :No such nick/channel"
#define		ERR_NORECIPIENT		" :No recipient given (PRIVMSG)"
#define		ERR_NORECIPIENT		" :No recipient given (PRIVMSG)"
class Channel;
class	PrivateMsg {
		private:
			std::string						input;
			std::string						hostName;
			std::string						nickName;
			std::string						userName;
			std::string						capter;
			std::string						msg;
			std::map<std::string, int>		clts;
			std::map<std::string,  Channel> channels;
		public:
			PrivateMsg(const std::string& ,const std::string&,const std::string&,const std::string&, std::map<std::string, int>&, std::map<std::string, Channel>&);
			bool					isRecipientExist( void );
			bool					isTextExist();
			bool					isCapterExist( void );
			bool					isValidCapter( void );
			std::string				Replay( const std::string& hint );
			const std::string&		getHostName();
			const std::string&		getNickName();
			const std::string&		getUserName();
			const std::string&		getCapter();
			const std::string&		getMsg();
			int						getFdCapter();
};
