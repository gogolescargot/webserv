/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:56 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/30 21:14:44 by marvin           ###   ########.fr       */
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
#include <fcntl.h>
#include <sys/select.h>

class Socket {
	public:
		Socket();
		~Socket();
		Socket(std::vector<Server*>);
        std::vector<int> const &getServerFD() const;
		std::vector<int> const &getClientFD() const;
	private:
		void launchSocket(Server *server);
        void eventLoop();

        std::vector<Server*> _servers;
        std::vector<int> _server_fds;
		std::vector<int> _client_fds;
        std::map<int, Server *> _server_map;
        std::map<int, Server*> _client_map;
		std::string			 _rawRequest;
};