/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCmd.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:13:15 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:13:16 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Cmd.hpp"

class topicCmd : public Cmd{
	std::string channel,cmd,topic;
public :
	topicCmd(std::string _input);
	std::string getChannelName();
	std::string getTopic();
};

