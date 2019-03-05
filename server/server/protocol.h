#ifndef PROTOCOL
#define PROTOCOL

constexpr int DATA_SIZE = 256;

struct DefaultProtocol
{
	int id;
	int op;
	int seq;
	char data[DATA_SIZE];
};

struct UserMoveProtocol
{
	int op = 1;
	int id;
	int seq;
	int pos;
};

#endif
