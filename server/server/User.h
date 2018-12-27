#pragma once
#include <cstring>

class User
{
public:
	User(int _fd, std::string _ip) { fd = _fd; ip = _ip; }

	std::size_t getHash() {
		return userHashFn(std::to_string(fd) + ip);
	}

	static std::size_t genHash(int _fd, std::string _ip) {
		std::hash<std::string> userHashFn;
		return userHashFn(std::to_string(_fd) + _ip);
	}
	int getFd() { return fd; }
	std::string getIp() { return ip; }

private:
	int fd;
	std::string ip;
	std::hash<std::string> userHashFn;
	// TODO: using seed to make hash
};
