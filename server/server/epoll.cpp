#include "epoll.h"

Epoll::Epoll()
{
	sock_fd = initSockFd();
	setNonBlock(sock_fd);
}

int Epoll::initSockFd()
{
	int fd;
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printError("Fail Open Socket fd");
		close(fd);
		exit(0);
	}

	int option = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);

	if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		printError("Fail Bind Socket");
		close(fd);
		exit(0);
	}

	listen(fd, KERNEL_TCP_BACKLOG);

	if (fd < 0) {
		printError("Fail Listen Socket");
		exit(0);
	}
	std::cout << "Listen Port at " << PORT << std::endl;
	return fd;
}

void Epoll::setNonBlock(int fd)
{
	int flags;
	flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1) {
		std::cout << "Fail Set Non Block" << std::endl;
	}
	else {
		std::cout << "Set Non Block" << std::endl;
		fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	}
}

void Epoll::run()
{
	int event_count;
	register int i;
	char read_buffer[READ_SIZE + 1];
	int bytes_read;
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
					// TODO : user�� ���� construct
					std::string client_ip(inet_ntoa(client_addr.sin_addr));
					User* user = new User(client_sock_fd, client_ip);
					userPool->addUser(user);
					userPool->print();
					if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock_fd, &event)) {
						std::cout << "Fail Add Epoll Client" << std::endl;
						userPool->delUser(user);
					}
				}
			}
			else {
				bytes_read = read(events[i].data.fd, read_buffer, READ_SIZE);
				if (bytes_read <= 0) {
					User* user = userPool->getUser(events[i].data.fd);
					userPool->delUser(user);
					userPool->print();
					close(events[i].data.fd);
					continue;
				}

				struct DefaultProtocol defaultProtocol;
				memcpy(&(defaultProtocol.id), read_buffer, bytes_read);
				messageQueue->push(events[i].data.fd, defaultProtocol);
			}
		}
	}
	close(epoll_fd);
}

void Epoll::printError(const std::string & errorStr)
{
	std::cout << "[Connection] : " << errorStr << std::endl;
}
