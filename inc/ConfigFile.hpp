/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggalon <ggalon@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 21:40:32 by ggalon            #+#    #+#             */
/*   Updated: 2024/06/20 18:36:35 by ggalon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

class Server;

class Location;

class ConfigFile
{
	private:
		const std::string _path;
		std::ifstream _stream;
		std::string _content;
		std::vector<std::string> _tokens;

	public:
		ConfigFile(const std::string&);
		~ConfigFile();

		std::string &getContent();

		int read();
		int open();
		int close();
		int token();
		int parse();

		std::string word(size_t &);
		int comment(size_t &);

		int createServer(size_t &);
		int checkToken(const size_t&, const std::string&);
		int checkTokenFunction(const size_t &, int (*func)(int));
		int isKeyword(size_t&);
		int getArgument(size_t&, Server&);
		int getMultipleArgument(size_t &i, Server &server);

		int checkErrorPages(size_t i);

		int getLocation(size_t &i, Server &server);
		int getArgumentLocation(size_t &i, Location &location);
		int getMultipleArgumentLocation(size_t &i, Location &location);

};
