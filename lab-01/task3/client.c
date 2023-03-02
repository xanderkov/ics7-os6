#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
char msg1[] = "Hello there!\n";

#define SOCK_NAME "SOCK_FILE"
#define SOCK_CLIENT "SOCK_CLIENT"

int main()
{
    int fd;
    struct sockaddr srvr_value, client_name;
    fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (fd < 0)
    {
        perror("socket");
        exit(1);
    }
    srvr_value.sa_family = AF_UNIX;
    strcpy(srvr_value.sa_data, SOCK_NAME);

    client_name.sa_family = AF_UNIX;
    strcpy(client_name.sa_data, SOCK_CLIENT);

    if (bind(fd, &client_name, sizeof(client_name)) == -1)
    {
        perror("bind");
        exit(1);
    }

    pid_t pid = getpid();

    char buf[1024];

    sprintf(buf, "%d\n", pid);

    if (sendto(fd, buf, strlen(buf) + 1, 0, &srvr_value, sizeof(srvr_value)) == -1)
    {
        perror("sendto");
        exit(1);
    }

    int bytes_read = recvfrom(fd, buf, 1024, 0, NULL, NULL);
    if (bytes_read == -1)
    {
        
        printf(buf);
        perror("recvfrom");
        exit(1);
    }
    else
    {
        buf[bytes_read] = '\0';
        printf(buf);
    }
    unlink(SOCK_CLIENT);
    close(fd);
    
    return 0;
}