#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <netinet/in.h>

#define SOCK_NAME "socket"
#define SOCK_NAME_CLIENT "socket_client"
#define BUF_SIZE 1024

int fd;
char recv_msg[BUF_SIZE];
char send_msg[BUF_SIZE];

void del_socket(void)
{
    if (close(fd) == -1)
    {
        printf("close() failed");
        return;
    }
    if (unlink(send_msg) == -1)
    {
        printf("unlink() returned -1");
    }
}


void sig_handler(int signum)
{
	printf("Catch SIGTSTP\n");
	close(fd);
    exit(0);
}


int main()
{
	int bytes;
	struct sockaddr_in server;

	memset(recv_msg, 0, BUF_SIZE);
	memset(send_msg, 0, BUF_SIZE);
	sprintf(send_msg, "%d", getpid());
	signal(SIGTSTP, sig_handler);

	server.sin_family = AF_INET;
    server.sin_port = htons(3425);
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		perror("socket() failed\n");
		exit(1);
	}

	// printf("fd: %d, server: %d, size: %ld", fd, (struct sockaddr*)&server, sizeof(server));
	if (connect(fd, (struct sockaddr*)&server, sizeof(server)) == -1)
	{
		perror("connect failed");
		exit(1);
	}

	while (1)
	{
		if(send(fd, send_msg, strlen(send_msg)+1, 0) == -1)
		{
			perror("send() failed\n");
		}

		printf("Child sent: %s\n", send_msg);
		memset(recv_msg, 0, BUF_SIZE);

		if((bytes = recv(fd, recv_msg, BUF_SIZE, 0)) > 0 )
		{
			printf("Client received: %s\n", recv_msg);
		}
		else
		{
			if(bytes < 0)
			{
				printf("recv() failed\n");
			}
			else
			{
				printf("Server socket closed \n");
				close(fd);
				break;
			}

		}
		sleep(3);
        printf("\n");
	}
	close(fd);

	return 0;
}