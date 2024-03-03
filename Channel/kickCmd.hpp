/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCmd.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:57 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:12:58 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

class kickCmd : public Cmd
{
public :
	std::string channel;
	std::vector<std::string> names;
	std::string				comment;
	kickCmd(std::string str);
	size_t	kickLen();
	void	setNames(std::string str);
	void	setComment();
};


