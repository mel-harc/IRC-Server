/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 22:49:53 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/23 02:24:37 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class	Client {
	private:
		std::string		realName;
		std::string		userName;
		std::string		nickName;
		int				fd;
		bool			pw;
		std::string		suspendRqst;
		bool			existSuspendRst;
		bool			isMode;
	public:
		Client( int fd );
		Client (void);
		void						setRealName( const std::string& );
		void						setUserName( const std::string& );
		void						setNickName( const std::string& );
		void						setSuspendRequest( const std::string& );
		void						setSockFd( int );
		void						setAuthenticate ( int );
		bool						isAuthenticate();
		bool						isExistSuspendRsqt();
		bool						isValidPass();
		std::string					notRegistred(const std::string&);
		int							getSockFd();
		const	std::string&		getNickName();
		const 	std::string&		getUserName();
		const 	std::string&		getRealName();
		std::string					getSuspendRqst();
		void						getters();
		bool						getMode();
		void						setMode(bool _mode);
		
};
