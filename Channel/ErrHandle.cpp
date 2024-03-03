/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrHandle.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-harc <mel-harc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 18:12:38 by mel-harc          #+#    #+#             */
/*   Updated: 2024/02/29 18:12:39 by mel-harc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

std::string ErrHnadle(int Error, ...)
{
	va_list ap;
	va_start(ap, Error);
	std::string msg;
	switch (Error)
	{
	case 461:
		msg = "461 " + std::string(va_arg(ap, char *)) + " :Not enough parameters";// "<client> <command> :Not enough parameters"
			break;
	case 403: //ERR_NOSUCHCHANNEL (403) "<client> <channel> :No such channel"
		msg = "403  " + std::string(va_arg(ap, char *)) + " :No such channel";
			break;
	case  482 ://ERR_CHANOPRIVSNEEDED (482)  "<client> <channel> :You're not channel operator"
		msg = "482  " + std::string(va_arg(ap, char *)) + " :You're not channel operator";//ERR_CHANOPRIVSNEEDED (482)  "<client> <channel> :You're not channel operator"
			break;
	case  442 : // ERR_NOTONCHANNEL (442) "<client> <channel> :You're not on that channel"
		msg = "442 " + std::string(va_arg(ap, char *)) + " :You're not on that channel"; // ERR_NOTONCHANNEL (442) "<client> <channel> :You're not on that channel"
			break;
	case  324 : //RPL_CHANNELMODEIS (324) "<channel> <mode> <mode params>"
		msg = "324 " + std::string(va_arg(ap, char *))+" "+std::string(va_arg(ap, char *))+" "+std::string(va_arg(ap, char *)); //RPL_CHANNELMODEIS (324) "<channel> <mode> <mode params>"
			break;
	case  467 : //ERR_KEYSET (467) "<channel> :Channel key already set"
		msg = "467 " + std::string(va_arg(ap, char *)) + " :Channel key already set"; //ERR_KEYSET (467) "<channel> :Channel key already set"
			break;
	case  502 : // ERR_USERSDONTMATCH (502) ":Cant change mode for other users"
		msg = "502 " + std::string(va_arg(ap, char *)) + ":Cant change mode for other users"; // ERR_USERSDONTMATCH (502) ":Cant change mode for other users"
			break;
	case 501 : //501 ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag"
		msg = ":Unknown MODE flag";//501 ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag"
			break;
	case 401 : // 401 ERR_NOSUCHNICK "<nickname> :No such nick/channel"
		msg = "401 " + std::string(va_arg(ap, char *)) + " :No such nick/channel";
			break;
	case 472 : // 472  ERR_UNKNOWNMODE "<char> :is unknown mode char to me"
		msg = "472 " + std::string(va_arg(ap, char *)) + " " + std::string(va_arg(ap, char *)) +" :is unknown mode char to me";
			break;
	case 331 : //  "<channel> :No topic is set"
		msg = "331 "+ std::string(va_arg(ap, char *)) + " " + std::string(va_arg(ap, char *)) + " :No topic is set";
			break;
	case 332 : // RPL_TOPIC "<channel> :<topic>"
		msg = "332 " + std::string(va_arg(ap, char *)) + " "+std::string(va_arg(ap, char *)) + " "+std::string(va_arg(ap, char *));
			break;
	case 441 : // ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel"
		msg = "441 " +std::string(va_arg(ap, char *)) + " " + std::string(va_arg(ap, char *)) + " :They aren't on that channel";
			break;
	case 451 :
		msg = ":" + std::string(va_arg(ap, char *)) + " 451 * :You have not registered";
			break;
	}
	va_end(ap);
	return msg;
}
