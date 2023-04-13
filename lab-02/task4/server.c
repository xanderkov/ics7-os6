#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define DEFAULT_PORT    64234
#define MAX_CONN        16
#define MAX_EVENTS      32
#define BUF_SIZE        16
#define MAX_LINE        256

int fd;

static void epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev;
	ev.events = events;
	ev.data.fd = fd;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
		perror("epoll_ctl()\n");
		exit(1);
	}
}


int main(void)
{	
	int i, n, epfd, nfds, listen_sock, conn_sock, socklen;
	char buf[BUF_SIZE];
	struct sockaddr_in srv_addr;
	struct sockaddr_in cli_addr;
	struct epoll_event events[MAX_EVENTS];

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	bzero((char *)&srv_addr, sizeof(struct sockaddr_in));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	srv_addr.sin_port = htons(DEFAULT_PORT);

	bind(listen_sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
	if (fcntl(listen_sock, F_SETFD, fcntl(listen_sock, F_GETFD, 0) | O_NONBLOCK) ==-1) {
		perror("fcntl error");
	}
	listen(listen_sock, MAX_CONN);

	epfd = epoll_create(1);
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
	ev.data.fd = listen_sock;
	if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
		perror("epoll_ctl()\n");
		exit(1);
	}

	socklen = sizeof(cli_addr);
	for (;;) {
		nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
		for (i = 0; i < nfds; i++) {
			if (events[i].data.fd == listen_sock) {
				conn_sock = accept(listen_sock, (struct sockaddr *)&cli_addr, &socklen);
				inet_ntop(AF_INET, (char *)&(cli_addr.sin_addr), buf, sizeof(cli_addr));
				printf("Client: %s:%d\n", buf, ntohs(cli_addr.sin_port));

				if (fcntl(conn_sock, F_SETFD, fcntl(conn_sock, F_GETFD, 0) | O_NONBLOCK) ==-1) {
					perror("fcntl error");
				}
				struct epoll_event ev;
				ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
				ev.data.fd = conn_sock;
				if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
					perror("epoll_ctl()\n");
					exit(1);
				}

			} else if (events[i].events & EPOLLIN) {
				bzero(buf, sizeof(buf));
				n = read(events[i].data.fd, buf, sizeof(buf));
				if (n <= 0) {
					break;
				} else {
					printf("Client pid: %s\n", buf);
					write(events[i].data.fd, buf, strlen(buf));
				}
			} else {
				printf("Error\n");
			}
			if (events[i].events & (EPOLLRDHUP | EPOLLHUP)) {
				printf("Connection closed\n");
				epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
				close(events[i].data.fd);
				continue;
			}
		}
	}
}