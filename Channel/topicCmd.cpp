/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCmd.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:13:10 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:13:11 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "topicCmd.hpp"

std::string topicCmd::getTopic()
{
	return this->topic;
}

std::string topicCmd::getChannelName()
{
	return this->channel;
}

topicCmd::topicCmd(std::string _input) : Cmd(_input)
{
	bool retFlag = 0;
	for (size_t i = 0; i < argsLen(); i++)
	{
		if(i ==  0)
			this->cmd = args[0];
		else if(i == 1)
			this->channel = args[1];
		else
		{
			if (i ==2 && args[2][0] != ':')
			{
				this->topic = args[2];
				return;
			}
			if (i == 2)
				args[2].erase(0,1);
			this->topic.append(args[i]);
			if (i < argsLen() && !retFlag)
				this->topic.append(" ");
		}
	}
}
