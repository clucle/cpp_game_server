#pragma once

#include "User.h"
#include <iostream>
#include <unordered_map>

class UserPool
{
public:
	void addUser(int fd, const char* ip) {
		User u(fd, ip);
		userPool.insert({ ip, u });
	}

	void delUser(const char* ip) {
		userPool.erase(ip);
	}

	void print() {
		for (auto it = userPool.begin(); it != userPool.end(); ++it)
			std::cout << " " << it->first << " " << it->second << std::endl;
	}

private:
	std::unordered_map<const char*, User> userPool;
};

