#ifndef USER
#define USER
#include <cstring>

class User
{
public:
	User(int _fd, std::string _ip) { fd = _fd; ip = _ip; }

	int getFd() { return fd; }
	std::string getIp() { return ip; }

private:
	int fd;
	std::string ip;
};
#endif
