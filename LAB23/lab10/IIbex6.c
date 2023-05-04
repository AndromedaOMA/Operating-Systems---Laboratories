#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    pid_t pid;
    FILE *f;
    int i = 0;
    char buf[100];

    f = fopen("pingpong.txt", "w");
    if (f == NULL)
    {
        perror("fopen");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }

    if (pid == 0)
        while (i < n)
        {
            while (fscanf(f, "%s", buf) == 0)
                sleep(1);
            printf("%s", buf);
            fflush(stdout);
            fprintf(f, "ho, ");
            fflush(f);
            i++;
        }
    else
        while (i < n)
        {
            fprintf(f, "hi-");
            fflush(f);
            while (fscanf(f, "%s", buf) == 0)
                sleep(1);
            printf("%s", buf);
            fflush(stdout);
            i++;
        }

    fclose(f);

    return 0;
}
