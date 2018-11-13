#include "server.h"

Server::Server()
{
	Connection conn;
	conn.runThread();
}

Server::~Server()
{
}
