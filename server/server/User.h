#pragma once
#include <cstring>

class User
{
public:
	User(int _fd, const char* _ip) { fd = _fd; strcpy(ip, _ip); }

	std::size_t getHash() {
		return ((std::hash<int>()(fd)) ^ (std::hash<const char *>()(ip)));
	}

	static std::size_t genHash(int _fd, const char* _ip) {
		return ((std::hash<int>()(_fd)) ^ (std::hash<const char *>()(_ip)));
	}
	int getFd() { return fd; }
	std::string getIp() { return ip; }

private:
	int fd;
	char ip[20];
};
