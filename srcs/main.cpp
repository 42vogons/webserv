/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 22:23:56 by anolivei          #+#    #+#             */
/*   Updated: 2023/04/21 18:38:46 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sockets.hpp"
#include <csignal>

Sockets sockets;

static void handle_signal(int signal)
{
	sockets.close_sockets();
	exit(signal);
}

int	main(void)
{
	signal(SIGINT, handle_signal);
	sockets.handleSocketConnections();
	return (0);
}
