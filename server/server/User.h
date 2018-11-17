#pragma once
#include <cstring>

class User
{
public:
	User(int _fd, const char* _ip) { fd = _fd; strcpy(ip, _ip); }

	std::size_t getHash() {
		return ((std::hash<int>()(fd)) ^ (std::hash<const char *>()(ip)));
	}

	int getFd() { return fd; }
	std::string getIp() { return ip; }

private:
	int fd;
	char ip[20];
};
