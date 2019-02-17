#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE


#include <queue>
#include <string>
#include <utility>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

#include "const.h"
#include "common.h"
#include "User.h"
#include "UserPool.h"

class MessageQueue
{
public:
	int size() { return (int)msgs.size(); }

	void push(int fd, char msg[]) {
		User* user = userPool->getUser(fd);
		std::cout << "byte reads : " << msg << '\n';
		std::cout << "read : " << unsigned(msg[0]) << '\n';
		std::cout << "read : " << unsigned(msg[1]) << '\n';

		std::lock_guard<std::mutex> lock(m);
		if (DEBUG) std::cout << "[QUEUE] ip : " << user->getIp() << " fd : " << user->getFd() << " msgs : " << msg << '\n';
		// msgs.push(std::make_pair(user, msg));
		c.notify_one();
	}

	void pop() {
		std::unique_lock<std::mutex> lock(m);
		while (msgs.empty())
		{
			c.wait(lock);
		}
		std::pair<User*, std::string> val = msgs.front();
		msgs.pop();

		User* &user = val.first;
		std::string msg = user->getRemainMsg() + val.second;
		// token split
		size_t pos = 0;
		std::string token;
		while ((pos = msg.find(delimiter)) != std::string::npos) {
			token = msg.substr(0, pos);
			std::cout << token << std::endl;
			std::string tokenMsg = std::to_string(user->getFd()) + " : " + token;
			userPool->broadcast(tokenMsg);
			if (DEBUG) std::cout << "[POP] : " << val.second << '\n';
			msg.erase(0, pos + delimiter.length());
		}
		user->setRemainMsg(msg);
	}

	void setUserPool(UserPool* _userPool) {
		userPool = _userPool;
	}

	std::pair<User*, std::string> front() { return msgs.front(); }

	void runThread() { m_thread = std::thread(&MessageQueue::run, this); };
	void joinThread() { m_thread.join(); };


private:
	std::queue<std::pair<User*, std::string>> msgs;
	mutable std::mutex m;
	std::condition_variable c;
	std::thread m_thread;
	UserPool* userPool;
	void run() {
		for (;;)
		{
			MessageQueue::pop();
		}
	}
	bool DEBUG = 1;
	std::string delimiter = ">~<";
};
#endif
