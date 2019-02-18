#ifndef USER
#define USER
#include <cstring>
#include <vector>

class User
{
public:
	User(int _fd, std::string _ip) {
		fd = _fd;
		ip = _ip;
		pos = 0;
		seq = 0;
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
	void moveRight() { pos += step; seq++; }
	void moveLeft() { pos -= step; seq++; }
	int getPos() { return pos; }
	int getSeq() { return seq; }

private:
	int fd;
	int pos;
	int seq;
	const int step = 10;
	std::string ip;
	std::vector<char> remainMsg;
};
#endif
