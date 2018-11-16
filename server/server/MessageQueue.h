#pragma once
#include <queue>
#include <string>

class MessageQueue
{
public:
	int size() { return (int)msgs.size(); }
	void push(char* msg) { msgs.push(msg); }
	void pop() { msgs.pop(); }
	char* front() { return msgs.front(); }

private:
	std::queue<char*> msgs;
};
