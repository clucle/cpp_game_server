#ifndef PROTOCOL
#define PROTOCOL

constexpr int DATA_SIZE = 256;

struct DefaultProtocol
{
	int op;
	int seq;
	char data[DATA_SIZE];
};

#endif
