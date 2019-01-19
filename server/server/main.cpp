#include "common.h"
#include "epoll.h"
// #include "MessageQueue.h"
// #include "Userpool.h"

int main()
{
	Epoll epoll;
	MessageQueue gMessageQueue;
	UserPool gUserPool;
	epoll.setUserPool(&gUserPool);
	epoll.setMessageQueue(&gMessageQueue);
	epoll.runThread();
	gMessageQueue.setUserPool(&gUserPool);
	gMessageQueue.runThread();
	gMessageQueue.joinThread();
	epoll.joinThread();
	return 0;
}
