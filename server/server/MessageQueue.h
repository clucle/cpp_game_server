#ifndef MESSAGE_QUEUE
#define MESSAGE_QUEUE


#include <queue>
#include <string>
#include <utility>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>
#include <bitset>

#include "const.h"
#include "common.h"
#include "User.h"
#include "Userpool.h"
#include "protocol.h"

class MessageQueue
{
public:
	int size() { return (int)msgs.size(); }

	void push(int fd, char msg[], int byte_read) {
		User* user = userPool->getUser(fd);
		std::lock_guard<std::mutex> lock(m);
		if (DEBUG) std::cout << "[QUEUE] ip : " << user->getIp() << " fd : " << user->getFd() << " msgs : " << msg << '\n';
		for (int i = 0; i < byte_read; ++i) {
			if (msg[i] == '\0') msg[i] = '/';
		}
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
		std::vector<char>* v = user->getRemainMsg();
		std::cout << (void*)&v << '\n';
		int start = 0;
		int delimSize = strlen(delimiter);

		for (size_t i = 0; i <= v->size() - delimSize; ++i) {
			int j;
			for (j = 0; j < delimSize; j++) {
				if (v->at(i + j) != delimiter[j]) break;
			}
			if (j == delimSize) {
				for (size_t p = start; p < i; p++) {
					if (v->at(p) == '/') v->at(p) = 1;
					// std::cout << signed(v[p]) << '\n';
					std::string bits = std::bitset<8>(v->at(p)).to_string();
					std::cout << bits << '\n';
				}
				
				Protocol *protocol = reinterpret_cast<Protocol*>(v->data());
				std::cout << v->data() << '\n';
				std::cout << "op : " << signed(protocol->op) << '\n';
				std::cout << "Seq : " << (int)protocol->seq << '\n';
				std::string bits = std::bitset<32>(protocol->seq).to_string();
				std::cout << bits << '\n';
				int op = (signed(v->at(start)) & 127);
				switch(op) {
				case 1:
					user->moveRight();
					break;
				case 2:
					user->moveLeft();
					break;
				}
				std::string msg = std::to_string(op) + std::to_string(user->getFd()) + std::to_string(user->getSeq()) + std::to_string(user->getPos());
				userPool->broadcast(user, msg);
				start = i + delimSize;
			}
		}

		char *rest = &v->at(start);
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
