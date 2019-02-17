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

		char *buffer = new char[v.size()];
		std::copy(v.begin(), v.end(), buffer);
		user->clearRemainMsg();

		char* p = strstr(buffer, delimiter);

		while (p) {
			// find delimiter at p;
			p[0] = '\0';
			std::cout << buffer;
			p[0] = '>';
		}
		// std::cout << "remain size : " << strlen(msg) << '\n';

		/*
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
		user->setRemainMsg(msg);*/
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
