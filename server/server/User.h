#pragma once
#include <cstring>
class User
{
public:
	User(int _fd, const char* _ip) { fd = _fd; strcpy(ip, _ip); }

	friend std::ostream & operator<<(std::ostream & _stream, User const & u) {
		_stream << std::to_string(u.fd);
		return _stream;
	}

	struct cmpUser
	{
		bool operator()(const char* a, const char* b) const
		{
			return std::strcmp(a, b) < 0;
		}
	};

private:
	int fd;
	char ip[20];
};
