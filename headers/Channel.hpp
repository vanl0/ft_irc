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
		std::string password;
		int limit;

		bool		inviteFlag;
		bool 		topicRights;
		bool		keyFlag;
		bool		operatorFlag;
		bool		limitFlag;

	public:
		Channel(const std::string &channelName);
		
		std::string		getName(void);
		std::set<int>	getMembers(void) const;
		std::string		getTopic(void) const;
		int				getLimit(void) const;

		bool			getInviteFlag(void) const;
		bool			getTopicRights(void) const;
		bool			getKeyFlag(void) const;
		bool			getOperatorFlag(void) const;
		bool			getLimitFlag(void) const;
		
		void			setInviteFlag(bool flag);
		void			setTopicRights(bool flag);
		void			setKeyFlag(bool flag);
		void			setOperatorFlag(bool flag);
		void			setLimitFlag(bool flag);

		void			setTopic(std::string const &topic);
		void			setPassword(std::string const &pass);
		void			setLimit(int newLimit);

		bool			isInChannel(int fd) const;
		void			addUser(int fd);
		void			removeUser(int fd);

		bool			isOperator(int fd) const;
		void			addOperator(int fd);
		void			removeOperator(int fd);


};

#endif