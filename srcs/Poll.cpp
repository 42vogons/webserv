/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpereira <cpereira@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/22 14:58:15 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/20 13:18:42 by cpereira         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Poll.hpp"

Poll::Poll(void) {
	return ;
}

Poll::Poll(const Poll& obj) {
	*this = obj;
	return ;
}

Poll::~Poll(void) {
	std::cout << "destruindo poll" << std::endl; 
	delete[] this->_socketsToWatch;
	return ;
}

void Poll::start(std::vector <Socket *> &sockets) {
	this->_size = sockets.size();
	this->_sockets = sockets;
	this->_socketsToWatch = new struct pollfd[_size];
	for (size_t i = 0; i < this->_size; i++) {
		this->_socketsToWatch[i].fd = sockets[i]->getServerFd();
		this->_socketsToWatch[i].events = POLLIN | POLLPRI | POLLOUT;
	}
}

void Poll::exec(void) {
	int ret = poll(this->_socketsToWatch, this->_size, 1000);
	if (ret == -1){
		throw (PollError());
	}
		
}

Socket* Poll::getSocket(size_t index) {
	return (this->_sockets[index]);
}

short Poll::getReturnEvents(size_t index) {
	return (this->_socketsToWatch[index].revents);
}

Poll& Poll::operator=(const Poll& obj) {
	if (this != &obj) {
		this->_size = obj._size;
		this->_sockets = obj._sockets;
		this->_socketsToWatch = obj._socketsToWatch;
	}
	return (*this);
}

std::ostream& operator<<(std::ostream& o, const Poll& i) {
	(void)i;
	o << "something";
	return o;
}
