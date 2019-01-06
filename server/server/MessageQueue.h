#pragma once
#include <queue>
#include <string>
#include <utility>

#include <iostream>
#include "User.h"

class MessageQueue
{
public:
	int size() { return (int)msgs.size(); }
	void push(User user, char* msg) {
		if (DEBUG) std::cout << "[QUEUE] ip : " << user.getIp() << " fd : " << user.getFd() << " hash : " << user.getHash() << " msgs : " << msg << '\n';
		msgs.push(std::make_pair(user, msg));
	}
	void pop() { msgs.pop(); }
	std::pair<User, char*> front() { return msgs.front(); }

private:
	std::queue<std::pair<User, char*>> msgs;
	bool DEBUG = 1;
};
