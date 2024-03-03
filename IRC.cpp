/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/11 19:02:43 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/19 23:10:36 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

int main(int ac, char** av)
{
	try {
		if (ac != 3)
			throw std::invalid_argument("invalid argument !");
		IrcServer	srv(av[1], av[2]);
		srv.createSocket();
		srv.setNonBloking();
		srv.bindSrv();
		srv.listningConnection();
	} catch ( const std::exception& e ){
		std::cerr << "\033[31m" << e.what()  << "\033[0m" << std::endl;
	}
}