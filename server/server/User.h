#ifndef USER
#define USER
#include <cstring>
#include <vector>

class User
{
public:
	User(int _fd, std::string _ip) {
		fd = _fd; ip = _ip;
	}

	int getFd() { return fd; }
	std::string getIp() { return ip; }
	std::vector<char> getRemainMsg() { return remainMsg; }
	void clearRemainMsg() { remainMsg.clear(); }
	void appendRemainMsg(char* msg) {
		for (size_t i = 0; i < strlen(msg); i++) {
			remainMsg.push_back(msg[i]);
		}
	}

private:
	int fd;
	std::string ip;
	std::vector<char> remainMsg;
};
#endif
