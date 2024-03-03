/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 21:40:43 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 19:01:49 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime> 

#define PASSWORD(X) (std::string("PASS ") + X + "\r\n")
#define NICKNAME	std::string("NICK bot\r\n")
#define USERNAME	std::string("USER bot * 0 bot\r\n")

class Bot {
	private:
		std::vector<std::string>	quotes;
		std::vector<std::string>	params;
		std::string 				apiBase;
    	std::string 				apiKey;
		std::string					pass;
		std::string					sender;
		std::string					hostName;
		int 						sockfd; 
		int							portno;
   		struct sockaddr_in 			serv_addr;
   		struct hostent 				*server;
	public:
		Bot( const std::string&, const std::string&);
		~Bot();
		void					creatSocket();
		void					setHostName();
		const std::string		getHostName();
		void					connectTOSrv();
		void					login();
		bool					isLoged();
		void					parseRequest(std::string input);
		void					storeQuotes();
		const std::string		getSender();
		std::string				getMsg();
		void					sendQuote();
		void					welcoming();
		void					cleanUp();
		std::string				getRequest();
};