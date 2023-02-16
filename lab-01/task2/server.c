#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#define SOCK_NAME "SOCK_FILE"



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
    char buf[1024];
    
    while(1)
    {
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
    }

    close(fd);

    return 0;
}