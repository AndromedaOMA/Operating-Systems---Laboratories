#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) 
{
    int k, n;
    pid_t pid, *child_pids;

    if (argc < 3) 
    {
        printf("Usage: %s K N\n", argv[0]);
        return 1;
    }
    k = atoi(argv[1]);
    n = atoi(argv[2]);

    // alocarea memoriei pentru vectorul de PID-uri ale copiilor
    child_pids = malloc(k * sizeof(pid_t));
    
    // crearea proceselor la nivelurile 1 până la N
    for (int i = 1; i <= n; i++) 
    {
        printf("Nivel %d:\n", i);
        for (int j = 1; j <= k; j++) 
        {
            pid = fork();
            if (pid == -1) 
            {
                printf("Error: nu s-a putut crea un proces copil!\n");
                return 1;
            }
            else 
            if (pid == 0) 
            {
                printf("Process (%d,%d): PID=%d, parent PID=%d\n", i, j, getpid(), getppid());
                exit(0);
            } 
            else 
                child_pids[j-1] = pid;
        }
        // așteptarea terminării proceselor fiu de la nivelul curent
        for (int j = 1; j <= k; j++)
            waitpid(child_pids[j-1], NULL, 0);
    }

    // eliberarea memoriei alocate
    free(child_pids);

    return 0;
}
