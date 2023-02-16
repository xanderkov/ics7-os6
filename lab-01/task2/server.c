#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#define SOCK_NAME "SOCK_FILE"

char buf[] = "Hello there!\n";


int main() 
{
    int fd;
    struct sockaddr srvr_name, client_name;
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(1);
    }
    srvr_name.sa_family = AF_UNIX;
    strcpy(srvr_name.sa_data, SOCK_NAME);

    if (bind(fd, &srvr_name, sizeof(srvr_name)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while(1)
    {
        if (recvfrom(fd, buf, 1024, 0, &srvr_name, sizeof(srvr_name)) == -1)
        {
            perror("recvfrom");
            exit(1);
        }
    }

    close(fd);

    return 0;
}