#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "utils.hpp"
# include <set>

class Channel
{
	private:
		std::string	name;
		std::set<int> members;
		std::set<int> operators;
		std::string topic;
	public:
		Channel(const std::string &channelName);
		
		std::string getName(void);

		bool isInChannel(int fd) const;
		void addUser(int fd);
		void removeUser(int fd);
		

		bool isOperator(int fd) const;
		void addOperator(int fd);
		void removeOperator(int fd);


};

#endif