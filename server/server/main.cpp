#include <cstdio>
#include "common.h"
#include "epoll.h"
#include "MessageQueue.h"

MessageQueue gMessageQueue;

int main()
{
	Epoll epoll;
	epoll.runThread();
    return 0;
}
