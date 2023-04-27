#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define SOCK_NAME_SERVER "socket_server"
#define SOCK_NAME_CLIENT "socket_client_"
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
 //   if (unlink(send_msg) == -1)
    {
        printf("unlink() returned -1");
    }
}


void sig_handler(int signum)
{
	printf("Catch SIGTSTP\n");
    del_socket();
    exit(0);
}


int main()
{
	int bytes;
	struct sockaddr server;
	struct sockaddr client;

	if((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1 )
	{
		printf("socket() failed\n");
		return EXIT_FAILURE;
	}

	memset(recv_msg, 0, BUF_SIZE);
	memset(send_msg, 0, BUF_SIZE);
	sprintf(send_msg, "%d", getpid());
	signal(SIGTSTP, sig_handler);

	server.sa_family = AF_UNIX;
	strcpy(server.sa_data, SOCK_NAME_SERVER);

	client.sa_family = AF_UNIX;
	strcpy(client.sa_data, send_msg);

	if(bind(fd, &client, sizeof(client)) == -1)
	{
		perror("bind() failed\n");
		del_socket();
		return EXIT_FAILURE;
	}

	if(sendto(fd, send_msg, strlen(send_msg)+1, 0, &server, sizeof(server)) == -1)
	{
		perror("sendto() failed\n");
	}

	printf ("Child sent: %s\n", send_msg);
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
		}

	}
	
	close(fd);

	return 0;
}