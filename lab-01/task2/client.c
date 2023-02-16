#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

char msg1[] = "Hello there!\n";

#define SOCK_NAME "SOCK_FILE"

int main()
{
    int fd;
    struct sockaddr srvr_value;
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(1);
    }
    srvr_value.sa_family = AF_UNIX;
    strcpy(srvr_value.sa_data, SOCK_NAME);

    pid_t pid = getpid();

    char buf[1024];

    sprintf(buf, "%d\n", pid);

    if (sendto(fd, buf, strlen(buf) + 1, 0, &srvr_value, sizeof(srvr_value)) == -1)
    {
        perror("sendto");
        exit(1);
    }


    close(fd);

    return 0;
}