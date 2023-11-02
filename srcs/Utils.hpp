/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anolivei <anolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:38:53 by anolivei          #+#    #+#             */
/*   Updated: 2023/11/02 16:38:54 by anolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

bool directoryExists(const char* path);
bool createDirectory(const char* path);

#endif