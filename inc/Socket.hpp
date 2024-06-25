/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 12:46:56 by lunagda           #+#    #+#             */
/*   Updated: 2024/06/26 00:09:09 by ggalon           ###   ########.fr       */
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
		void launchSocket(const Server&);
		int const &getServerFD() const;
		std::vector<int> const &getClientFD() const;
	private:
		std::vector<Server> _servers;
		int _server_fd;
		std::vector<int> _client_fds;
};