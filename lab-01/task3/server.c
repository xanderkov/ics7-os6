#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#define SOCK_NAME "SOCK_FILE"
#define SOCK_CLIENT "SOCK_CLIENT"



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

    pid_t pid_srv = getpid();

    char buf_srv[1024];

    sprintf(buf_srv, "%d\n", pid_srv);

    printf("Server pid: ");
    printf(buf_srv);
    
    while (1)
    {
        int size_client = sizeof(client_name);
        int bytes_read = recvfrom(fd, buf, 1024, 0, &client_name, &size_client);
        if (bytes_read == -1)
        {
            
            printf(buf);
            perror("recvfrom");
            exit(1);
        }
        else
        {
            buf[bytes_read] = '\n\0';
            printf(buf);
        }

        if (sendto(fd, buf_srv, strlen(buf_srv) + 1, 0, &client_name, sizeof(client_name)) == -1)
        {
            perror("sendto");
            exit(1);
        }
        
    }

    close(fd);

    return 0;
}