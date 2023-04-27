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

void del_socket(void)
{
    if (close(fd) == -1)
    {
        printf("close() failed");
        return;
    }
    if (unlink(SOCK_NAME_SERVER) == -1)
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

int main(void)
{
    struct sockaddr server, client;
    socklen_t client_len = sizeof(client);

    if ((fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket() failed");
        return EXIT_FAILURE;
    }
    
    char buf[1024];
    char msg[100];
    sprintf(msg, " Hello from server: pid %d", getpid());
    int  bytes;

    

    server.sa_family = AF_UNIX;
    strcpy(server.sa_data, SOCK_NAME_SERVER);

    if (bind(fd, &server, sizeof(server)) == -1)
    {
        perror("bind failed");
        del_socket();
        return EXIT_FAILURE;
    }

    signal(SIGTSTP, sig_handler);

    printf("Waiting for messages.\nCtrl + Z to stop...\n\n");

    while (1)
    {
        bytes = recvfrom(fd, buf, 1024, 0, &client, &client_len);
        if (bytes < 0)
        {
            del_socket();
            perror("recvfrom() failed: ");
            return EXIT_FAILURE;
        }
        buf[bytes] = 0;
        printf("Server recieved: %s\n", buf);
        strcat(buf, msg);
        sendto(fd, buf, strlen(buf)+1, 0, &client, client_len);
        memset(buf, 0, BUF_SIZE);
    }

    del_socket();
    return EXIT_SUCCESS;
}