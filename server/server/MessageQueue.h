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

	void push(User user, char* msg) {
		std::lock_guard<std::mutex> lock(m);
		if (DEBUG) std::cout << "[QUEUE] ip : " << user.getIp() << " fd : " << user.getFd() << " hash : " << user.getHash() << " msgs : " << msg << '\n';
		msgs.push(std::make_pair(user, msg));
		c.notify_one();
	}

	void pop() {
		std::unique_lock<std::mutex> lock(m);
		while (msgs.empty())
		{
			c.wait(lock);
		}
		std::pair<User, char*> val = msgs.front();
		msgs.pop();
		// do something
		if (DEBUG) std::cout << "[POP] : " << val.second << '\n';
		std::string msg = std::to_string(val.first.getFd()) + " : " + std::string(val.second);
		userPool->broadcast(msg);
	}

	void setUserPool(UserPool* _userPool) {
		userPool = _userPool;
	}

	std::pair<User, char*> front() { return msgs.front(); }

	void runThread() { m_thread = std::thread(&MessageQueue::run, this); };
	void joinThread() { m_thread.join(); };


private:
	std::queue<std::pair<User, char*>> msgs;
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
};
#endif
