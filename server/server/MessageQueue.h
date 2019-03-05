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
#include "protocol.h"

class MessageQueue
{
public:
	int size() { return (int)msgs.size(); }

	void push(int fd, DefaultProtocol defaultProtocol) {
		User* user = userPool->getUser(fd);
		std::lock_guard<std::mutex> lock(m);

		if (DEBUG) std::cout << "[QUEUE] ip : " << user->getIp() << " fd : " << user->getFd() << '\n';
		if (DEBUG) {
			std::cout << "id : " << defaultProtocol.id << '\n';
			std::cout << "op : " << defaultProtocol.op << '\n';
			std::cout << "seq : " << defaultProtocol.seq << '\n';
			std::cout << defaultProtocol.data << '\n';
		}
		msgs.push(std::make_pair(user, defaultProtocol));
		c.notify_one();
	}

	void pop() {
		std::unique_lock<std::mutex> lock(m);
		while (msgs.empty())
		{
			c.wait(lock);
		}
		
		std::pair<User*, DefaultProtocol> msg = msgs.front();
		msgs.pop();
		User* &user = msg.first;
		DefaultProtocol defaultProtocol = msg.second;

		int op = defaultProtocol.op;
		switch (op) {
		case 1:
			user->moveRight();
			break;
		case 2:
			user->moveLeft();
			break;
		}

		UserMoveProtocol data;
		data.id = defaultProtocol.id;
		data.seq = defaultProtocol.seq;
		data.pos = user->getPos();
		userPool->broadcast(user, (char*)reinterpret_cast<char*>(&data));
	}

	void setUserPool(UserPool* _userPool) {
		userPool = _userPool;
	}

	std::pair<User*, DefaultProtocol> front() { return msgs.front(); }

	void runThread() { m_thread = std::thread(&MessageQueue::run, this); };
	void joinThread() { m_thread.join(); };


private:
	std::queue<std::pair<User*, DefaultProtocol>> msgs;
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
	const char* delimiter = ">~<";
};
#endif
