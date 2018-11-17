#pragma once

#include "User.h"
#include <iostream>
#include <unordered_map>

class UserPool
{
public:
	void addUser(int fd, const char* ip) {
		User user(fd, ip);
		userPool.insert({ user.getHash(), user });
	}

	void addUser(User user) {
		userPool.insert({ user.getHash(), user });
	}

	void delUser(size_t key) {
		userPool.erase(key);
	}

	void delUser(User user) {
		userPool.erase(user.getHash());
	}

	void print() {
		for (auto it = userPool.begin(); it != userPool.end(); ++it)
			std::cout << " " << it->first << " " << it->second.getFd() << " " << it->second.getIp() << " " << it->second.getHash() << std::endl;
	}

private:
	std::unordered_map<size_t, User> userPool;
};

