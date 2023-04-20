#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#define BUF_SIZE 1024

int proc_env(char *path) {
    int len;
    char buf[BUF_SIZE];
    FILE *f = fopen(path, "r"); //"/proc/self/environ"
    while ((len = fread(buf, 1, BUF_SIZE, f)) > 0)
    {
        for (int i = 0; i < len; i++)
   		    if (buf[i] == 0)
      		    buf[i] = 10;
        buf[len - 1] = 0;
        printf("%s", buf);
    }
    fclose(f);
}

int set_stat(char *path) {
    char buf[BUF_SIZE];
    FILE *f = fopen(path, "r"); // "/proc/self/stat"
    fread(buf, 1, BUF_SIZE, f);
    char *pch = strtok(buf, " ");
    
    printf("stat: \n");
    while(pch != NULL)
    {
        printf("%s\n", pch);
        pch = strtok(NULL, " ");
    }
    fclose(f);
}

void read_fd(char *path_fd)
{
    printf("\nfd:\n");
    struct dirent *dirp;
    DIR *dp;
    char str[BUF_SIZE];
    char path[BUF_SIZE];
    dp = opendir(path_fd);//открыть директорию
    while ((dirp = readdir(dp)) != NULL)//читаем директорию
    {
        if ((strcmp(dirp->d_name, ".") != 0) &&
            (strcmp(dirp->d_name, "..") != 0))
        {
            sprintf(path, "%s%s", "/proc/self/fd/", dirp->d_name);
            readlink(path, str, BUF_SIZE);
            printf("%s -> %s\n", dirp->d_name, str);
        }
    }
    closedir(dp);
}


int main(int *argc, char **argv) 
{
    setbuf(stdout, NULL);
    int ch = -1;

    char env_path[1024];
    char stat_path[1024]; 
    char fd_path[1024];

    sprintf(env_path, "/proc/%s/environ", argv[1]);
    sprintf(stat_path, "/proc/%s/stat", argv[1]);
    sprintf(fd_path, "/proc/%s/fd/", argv[1]);
    
    proc_env(env_path);
    set_stat(stat_path);
    read_fd(fd_path);

    return 0;
}