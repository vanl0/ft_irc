#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include <set>

class Channel
{
	private:
		std::string	name;
		std::set<int> members;
		std::string topic;
	public:
		Channel(const std::string &channelName) : name(channelName) {};
		


};

#endif