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
		std::string					name;
		std::vector<Client *>		members;
		std::vector<Client *> 		operators;
		std::vector<std::string>	invitedUsers;
		std::string 				topic;
		std::string 				password;
		int 						userLimit;

		bool						inviteFlag;
		bool 						topicRights;
		bool						keyFlag;
		bool						operatorFlag;
		bool						limitFlag;

	public:
		Channel(const std::string &channelName);
		
		std::string				getName(void) const;
		std::vector<Client *>	getMembers(void) const;
		std::string				getTopic(void) const;

		bool					getInviteFlag(void) const;
		bool					getTopicRights(void) const;
		bool					getKeyFlag(void) const;
		
		bool					getLimitFlag(void) const;
		int						canUserJoin(void) const;
		
		void					setInviteFlag(bool flag);
		void					setTopicRights(bool flag);

		void					setTopic(std::string const &topic);
		bool					isPassValid(std::string const &pass) const;

		bool					isInChannel(Client *client) const;
		bool					isInChannel(const std::string &nick) const;
		Client					*getClient(std::string const &nick) const;
		
		void					addUser(Client *client);
		void					removeUser(Client *client);

		bool					isOperator(Client *client) const;
		void					addOperator(Client *client);
		void					removeOperator(Client *client);

		bool					isInvited(const std::string &nick) const;
		void					addInvited(const std::string &nick);
		void					removeInvited(const std::string &nick);
//------MODE
		void 					inviteMode(Client *client, bool plusminus);
		void					topicMode(Client *client, bool plusminus);
		void 					keyMode(Client *client, bool plusminus, std::string &key);
		void 					operatorMode(Client *client, bool plusminus, std::string &user);
		void 					limitMode(Client *client, bool plusminus, std::string &limit);
//------MESSAGES
		void					sendtoMembers(std::string const &msg, char const *color);
		void 					sendtoMembers(Client *client, const std::string &msg);

};

#endif