#include "common.h"
#include "epoll.h"
// #include "MessageQueue.h"
// #include "Userpool.h"

MessageQueue gMessageQueue;
UserPool gUserPool;

int main()
{
	Epoll epoll;
	epoll.runThread();
    return 0;
}
