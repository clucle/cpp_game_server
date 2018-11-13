#pragma once
#include "const.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <thread>
#include <iostream>
#include <string>
#include <unistd.h>
#include <string.h>

#include <sys/epoll.h>

class Connection
{
public:
	Connection();
	Connection(Connection const &) = delete;
	~Connection() { m_thread.join(); close(sock_fd); };
	void runThread() { m_thread = std::thread(&Connection::run, this); };

private:
	const int READ_SIZE = 1024;
	const int MAX_EVNETS = 1024;
	const int MAX_CLIENTS = 1024;
	const int EPOLL_TIME_OUT = 1024;

	int sock_fd;

	std::thread m_thread;
	void run() const;
	void printError(const std::string& errorStr);
};

