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
#include "Userpool.h"

class MessageQueue
{
public:
	int size() { return (int)msgs.size(); }

	void push(int fd, char msg[]) {
		User* user = userPool->getUser(fd);
		std::lock_guard<std::mutex> lock(m);
		if (DEBUG) std::cout << "[QUEUE] ip : " << user->getIp() << " fd : " << user->getFd() << " msgs : " << msg << '\n';
		msgs.push(std::make_pair(user, msg));
		c.notify_one();
	}

	void pop() {
		std::unique_lock<std::mutex> lock(m);
		while (msgs.empty())
		{
			c.wait(lock);
		}
		std::pair<User*, char*> val = msgs.front();
		msgs.pop();
		char* data = val.second;

		User* &user = val.first;
		user->appendRemainMsg(data);
		std::vector<char> v = user->getRemainMsg();

		int start = 0;
		int delimSize = strlen(delimiter);

		for (size_t i = 0; i <= v.size() - delimSize; ++i) {
			int j;
			for (j = 0; j < delimSize; j++) {
				if (v[i + j] != delimiter[j]) break;
			}
			if (j == delimSize) {
				for (size_t p = start; p < i; p++) {
					// do something
					// start ~ i - 1
					std::cout << unsigned(v[p]) << '\n';
				}

				start = i + delimSize;
			}
		}

		char *rest = &v[start];
		user->clearRemainMsg();
		user->appendRemainMsg(rest);
	}

	void setUserPool(UserPool* _userPool) {
		userPool = _userPool;
	}

	std::pair<User*, std::string> front() { return msgs.front(); }

	void runThread() { m_thread = std::thread(&MessageQueue::run, this); };
	void joinThread() { m_thread.join(); };


private:
	std::queue<std::pair<User*, char*>> msgs;
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
	const char* delimiter = ">~<";
};
#endif
