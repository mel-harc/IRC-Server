/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 23:11:01 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 16:55:47 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main(int ac, char **av)
{
	try{
		if (ac != 3)
			throw std::invalid_argument("Error: invalid args");
		Bot		myBot(av[1], av[2]);
		myBot.creatSocket();
		myBot.setHostName();
		myBot.connectTOSrv();
		myBot.login();
		if (!myBot.isLoged())
			throw std::invalid_argument("Wrong Information");
		myBot.storeQuotes();
		while (1)
		{
			std::string	str = myBot.getRequest();
			if (str.empty())
				break;
			myBot.parseRequest(str);
			if (myBot.getMsg() == "I WANT")
				myBot.sendQuote();
			else
				myBot.welcoming();
			myBot.cleanUp();
		}
	} catch ( const std::exception& e){
		std::cerr  << e.what()  << std::endl;
	}
}