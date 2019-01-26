#ifndef USER_POOL
#define USER_POOL

#include "User.h"
#include <string>
#include <unistd.h>
#include <iostream>
#include <unordered_map>

class UserPool
{
public:
	void addUser(User* user) {
		userPool.insert({ user->getFd(), user });
	}

	void delUser(User* user) {
		userPool.erase(user->getFd());
	}

	User* getUser(int fd) {
		std::unordered_map<int, User*>::iterator iter;
		iter = userPool.find(fd);
		if (iter != userPool.end()) {
			return userPool.at(fd);
		}
		std::cout << "Wrong : key " << fd << '\n';
		return userPool.at(fd);
	}

	void print() {
		for (auto it = userPool.begin(); it != userPool.end(); ++it)
			std::cout << " " << it->first << " " << it->second->getFd() << " " << it->second->getIp() << std::endl;
	}

	void broadcast(std::string msg) {
		for (auto it = userPool.begin(); it != userPool.end(); ++it) {
			write(it->second->getFd(), &msg[0], msg.length());
		}
	}

private:
	std::unordered_map<int, User*> userPool;
};
#endif
