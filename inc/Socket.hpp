/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lunagda <lunagda@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:56 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/25 14:29:35 by lunagda          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include "Server.hpp"
#include <cstring>
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>

class Socket {
	public:
		Socket();
		~Socket();
		Socket(std::map<std::string, Server>);
		void launchSocket(Server);
		int const &getServerFD() const;
		int const &getClientFD() const;
	private:
		std::vector<Server> _servers;
		int _server_fd;
		int _client_fd;
};