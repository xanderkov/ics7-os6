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

#define SOCK_NAME "socket"
#define SOCK_NAME_CLIENT "socket2"
#define BUF_SIZE 1024
#define N_CLIENTS 5

int fd;

void sig_handler(int signum)
{
	printf("Catch SIGTSTP\n");
    close(fd);
    exit(0);
}

int main(void)
{
    struct sockaddr_in server, client;
    socklen_t client_len = sizeof(client);
    char buf[1024];
    char msg[100];
    sprintf(msg, " Greetings from server with pid %d", getpid());
    int  bytes;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket() failed");
        exit(1);
    }

    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
    {
        printf("fcntl failed");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(3425);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        perror("bind failed");
        close(fd);
        exit(1);
    }

    signal(SIGTSTP, sig_handler);

    listen(fd, N_CLIENTS);
    int clients[N_CLIENTS] = {0};

    struct timeval timeout;
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    printf("Waiting for messages.\nCtrl + Z to stop...\n\n");

    while (1)
    {
        fd_set clientset;
        FD_ZERO(&clientset);
        FD_SET(fd, &clientset);

        for (int i = 0; i < N_CLIENTS; i++)
        {
            FD_SET(clients[i], &clientset);
        }

        int mx = 0;
        for (int i = 0; i < N_CLIENTS; i++)
        {
            if (clients[i] > mx)
                mx = clients[i];
        }
        if (fd > mx)
            mx = fd;

        if (select(mx + 1, &clientset, NULL, NULL, &timeout) == -1)
        {
            perror("select failed");
            exit(1);
        }

        if (FD_ISSET(fd, &clientset))
        {
            int sock = accept(fd, NULL, NULL);
            if (sock == -1)
            {
                perror("accept failed");
                close(fd);
                exit(1);
            }
            fcntl(sock, F_SETFL, O_NONBLOCK);
            int index = -2;
            for (int i = 0; i < N_CLIENTS; i++)
            {
                if (clients[i] == sock)
                    index = -1;
                else if (index != -1 && index == -2 && clients[i] == 0)
                    index = i;
            }
            if (index >= 0)
                clients[index] = sock;
        }

        for (int i = 0; i < N_CLIENTS; i++)
        {
            if(FD_ISSET(clients[i], &clientset))
            {
                bytes = recv(clients[i], buf, 1024, 0);
                if (bytes <= 0)
                {
                    close(clients[i]);
                    clients[i] = 0;
                    printf("recvfrom() failed");
                }
                else
                {
                    buf[bytes] = 0;
                    printf("Server recieved: %s\n", buf);
                    strcat(buf, msg);
                    if (send(clients[i], buf, strlen(buf)+1, 0) == -1)
                    {
                        perror("send to client failed");
                    }
                }
                memset(buf, 0, BUF_SIZE);
            }
        }

    }
    close(fd);
    return EXIT_SUCCESS;
}
