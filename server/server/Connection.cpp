#include "Connection.h"

Connection::Connection()
{
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printError("Fail Open Socket fd");
		close(sock_fd);
		exit(0);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	if (bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printError("Fail Bind Socket");
		close(sock_fd);
		exit(0);
	}

	listen(sock_fd, KERNEL_TCP_BACKLOG);

	if (sock_fd < 0) {
		printError("Fail Listen Socket");
		exit(0);
	}

	std::cout << "Listen Port at " << PORT << std::endl;
}


Connection::~Connection()
{
	close(sock_fd);
}

void Connection::run()
{

}

void Connection::printError(const std::string & errorStr)
{
	std::cout << "[Connection] : " << errorStr << std::endl;
}
