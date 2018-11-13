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

class Connection
{
public:
	Connection();
	~Connection();
	void run();
	void stop() { is_run = false; };

private:
	int sock_fd;
	bool is_run = false;
	void printError(const std::string& errorStr);
};

