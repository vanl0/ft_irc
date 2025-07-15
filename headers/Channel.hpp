#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# define USER_MAX 100
# include "utils.hpp"
# include <set>
# include <vector>
# include <algorithm>
# include "Client.hpp"
# include "utils.hpp"

class Channel
{
	private:
		std::string				name;
		std::vector<Client *>	members;
		std::vector<Client *> 	operators;
		std::string 			topic;
		std::string 			password;
		int 					limit;

		bool					inviteFlag;
		bool 					topicRights;
		bool					keyFlag;
		bool					operatorFlag;
		bool					limitFlag;

	public:
		Channel(const std::string &channelName);
		
		std::string				getName(void);
		std::vector<Client *>	getMembers(void) const;
		std::string				getTopic(void) const;
		int						getLimit(void) const;

		bool					getInviteFlag(void) const;
		bool					getTopicRights(void) const;
		bool					getKeyFlag(void) const;
		bool					getOperatorFlag(void) const;
		bool					getLimitFlag(void) const;
		
		void					setInviteFlag(bool flag);
		void					setTopicRights(bool flag);
		void					setKeyFlag(bool flag);
		void					setOperatorFlag(bool flag);
		void					setLimitFlag(bool flag);

		void					setTopic(std::string const &topic);
		void					setPassword(std::string const &pass);
		void					setLimit(int newLimit);

		bool					isInChannel(Client *client) const;
		bool					isInChannel(const std::string &nick) const;
		Client					*getClient(std::string const &nick) const;
		
		void					addUser(Client *client);
		void					removeUser(Client *client);

		bool					isOperator(Client *client) const;
		void					addOperator(Client *client);
		void					removeOperator(Client *client);

		void 					invite(Client *client, bool plusminus);
		void					topicMode(Client *client, bool plusminus);
		void 					keyMode(Client *client, bool plusminus, std::string &key);
		void 					operatorMode(Client *client, bool plusminus, std::string &user);
		void 					limitMode(Client *client, bool plusminus, std::string &limit);

		void					sendtoMembers(std::string const &msg, char const *color);
		void 					sendtoMembers(Client *client, const std::string &msg);

};

#endif