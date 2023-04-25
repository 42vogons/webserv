/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/24 22:53:57 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include <csignal>

WebServer web_server;

static void handle_signal(int signal)
{
	exit(signal);
}

int	main(void)
{
	signal(SIGINT, handle_signal);
	web_server.handleSocketConnections();
	return (0);
}
