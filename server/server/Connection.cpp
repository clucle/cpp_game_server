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

void Connection::run() const
{
	int event_count;
	register int i;
	char read_buffer[READ_SIZE + 1];
	size_t bytes_read;
	struct epoll_event event;
	struct epoll_event events[MAX_EVNETS];

	int epoll_fd;
	if ((epoll_fd = epoll_create(MAX_EVNETS)) < 0) {
		return;
	}

	event.events = EPOLLIN;
	event.data.fd = sock_fd;
	
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &event)) {
		close(sock_fd);
		close(epoll_fd);
		return;
	}

	int client_sock_fd;
	struct sockaddr_in client_addr;
	uint32_t client_len = sizeof(client_addr);

	std::cout << "running ... " << std::endl;

	while (true) {
		event_count = epoll_wait(epoll_fd, events, MAX_EVNETS, EPOLL_TIME_OUT);
		if (event_count < 0) {
			break;
		}
		for (i = 0; i < event_count; i++) {
			if (events[i].data.fd == sock_fd) {
				client_sock_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &client_len);
				if (client_sock_fd < 0) {
					close(sock_fd);
					close(epoll_fd);
					return;
				}
				else {

					struct epoll_event event;
					event.events = EPOLLIN;
					event.data.fd = client_sock_fd;

					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock_fd, &event)) {
						close(sock_fd);
						close(epoll_fd);
						return;
					}
					// user add
				}
			}
			else {
				bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
				if (bytes_read <= 0) {
					close(events[i].data.fd);
					continue;
				}
				read_buffer[bytes_read] = '\0';
				write(events[i].data.fd, read_buffer, bytes_read);
				std::cout << read_buffer << std::endl;
			}
		}
	}
	close(epoll_fd);
}

void Connection::printError(const std::string & errorStr)
{
	std::cout << "[Connection] : " << errorStr << std::endl;
}
