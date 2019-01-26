#ifndef USER
#define USER
#include <cstring>

class User
{
public:
	User(int _fd, std::string _ip) {
		fd = _fd; ip = _ip; remainMsg = "";
	}

	int getFd() { return fd; }
	std::string getIp() { return ip; }
	std::string getRemainMsg() { return remainMsg; }
	void setRemainMsg(std::string msg) { remainMsg = msg; }

private:
	int fd;
	std::string ip;
	std::string remainMsg;
};
#endif
