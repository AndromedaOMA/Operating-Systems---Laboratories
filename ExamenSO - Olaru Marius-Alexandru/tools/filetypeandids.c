#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void print_usage()
{
    printf("Usage: ./filetypeandids.c ACCOUNTID (+/-)SUM\n");
    exit(EXIT_FAILURE);
}

int main(int nr, char *a[])
{
    //1
    if (nr != 2)
    {
        printf("numar invalid de argumente\n");
        print_usage();
        exit(10);
    }
    
    //2
    if (access(a[1], F_OK) == -1)
    {
        perror("access");
        exit(11);
    }
    printf("%d:%d\n", getuid(), getgid());

    //3
    printf("%s", a[1]);

    if ((access(a[1], R_OK) == 0) && (access(a[1], W_OK) == 0) && (access(a[1], X_OK) == 0))
        printf(" : ");
    else
        exit(1);
        
    exit(0);
}
