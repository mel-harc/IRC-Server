/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 21:40:52 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 16:56:34 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot( const std::string& port, const std::string& pass ){
	if (pass.empty())
		throw std::runtime_error("Error: Invalid args");
	this->pass = pass;
	this->portno = atoi(port.c_str());
	std::memset((char *) &serv_addr, 0, sizeof(serv_addr));
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) != 0)
		throw std::runtime_error("Cannot get Host Name");
	this->hostName = hostname;
}
Bot::~Bot(){ close(sockfd); }
const std::string	Bot::getHostName(){ return this->hostName; }

void		Bot::welcoming(){
	std::string		replay;
	replay = "PRIVMSG " + getSender() + " :" + "HELLO, IF YOU WANT TO READ A QUOTE JUST TELL ME < I WANT >" + "\r\n";
	send(sockfd, replay.c_str(), replay.size(), 0);
}

void		Bot::sendQuote(){
	std::string		replay;
	srand(time(0));
    int index = rand() % quotes.size();
	replay = "PRIVMSG " + getSender() + " :" + quotes[index] + "\r\n";
	send(sockfd, replay.c_str(), replay.size(), 0);
}

void	Bot::storeQuotes(){
	std::ifstream	inputFile("bot/quote.csv");
	if (!inputFile)
		throw std::runtime_error("file");
	std::string		tmp;
	std::getline(inputFile, tmp);
	while (std::getline(inputFile, tmp))
		quotes.push_back(tmp);
	inputFile.close();
}

void	Bot::parseRequest( std::string input )
{
	size_t	index = 0;
	std::string		token;
	if (input.rfind('\n') != std::string::npos)
		input.erase(input.rfind('\n'), 1);
	if (input.rfind('\r') != std::string::npos)
		input.erase(input.rfind('\r'), 1);
	while ((index = input.find(' ')) != std::string::npos)
	{
		token = input.substr(0, index);
		params.push_back(token);
		input.erase(0, index + 1);
	}
	if (!input.empty())
		params.push_back(input);
}

std::string	Bot::getMsg(){
	std::string		msg(params[3]);
	for (size_t i = 4; i < params.size(); i++)
		msg += (" " + params[i]);
	if (msg.find(':', 0) != std::string::npos)
		msg.erase(0, 1);
	if (msg.rfind('\n') != std::string::npos)
		msg.erase(msg.rfind('\n'), 1);
	if (msg.rfind('\r') != std::string::npos)
		msg.erase(msg.rfind('\r'), 1);
	return msg;
}

const std::string	Bot::getSender(){
	params[0].erase(0, 1);
	std::string		nickName(params[0].substr(0, params[0].find('!')));
	return nickName;
}

void		Bot::cleanUp(){
	params.clear();
	sender.clear();
}

void		Bot::creatSocket(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw	std::runtime_error("ERROR opening socket of Bot");
}

void	Bot::setHostName(){
	this->server = gethostbyname("localhost");
	if (server == NULL) 
		throw	std::runtime_error("ERROR, no such host Bot");
	std::memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
	serv_addr.sin_family = AF_INET;
}

void	Bot::connectTOSrv(){
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
		throw	std::runtime_error("Error To Connecting To Server");
}

void	Bot::login(){
	std::string PASS = PASSWORD(pass);
	std::string NICK = NICKNAME;
	std::string USER = USERNAME;
	std::string AUTHENTICATE = PASS + NICK + USER;
	send(sockfd, AUTHENTICATE.c_str(), AUTHENTICATE.size(), 0);
}

std::string		Bot::getRequest(){
	char buffer[512];
	std::memset(buffer, 0, 512);
	if (recv(sockfd, buffer, sizeof(buffer) - 1, 0) <= 0)
		return "";
	std::string		request(buffer);
	return request;
}

bool	Bot::isLoged(){ return (getRequest().find("Welcome") != std::string::npos) ? true : false; }