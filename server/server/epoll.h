#ifndef EPOLL
#define EPOLL

#include "const.h"
#include "common.h"
#include "MessageQueue.h"
#include "protocol.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <fcntl.h>
#include <thread>
#include <string>
#include <unistd.h>
#include <string.h>

#include <sys/epoll.h>

class Epoll
{
public:
	Epoll();
	Epoll(Epoll const &) = delete;
	~Epoll() { m_thread.join(); close(sock_fd); };
	void runThread() { m_thread = std::thread(&Epoll::run, this); };
	void joinThread() { m_thread.join(); };
	void setUserPool(UserPool* _userPool) { userPool = _userPool; };
	void setMessageQueue(MessageQueue* _messageQueue) { messageQueue = _messageQueue; };

private:
	const int READ_SIZE = 1024;
	const int MAX_EVNETS = 1024;
	const int MAX_CLIENTS = 1024;
	const int EPOLL_TIME_OUT = 1024;

	int sock_fd;

	std::thread m_thread;

	UserPool* userPool;
	MessageQueue* messageQueue;

	int initSockFd();
	void setNonBlock(int fd);
	void run();
	void printError(const std::string& errorStr);
};
#endif
