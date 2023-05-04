#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main(void)
{
    int i, j;
    int i1, i2, codterm;
    pid_t pid, pid2;

    i1=1; i2 = 1;

    printf("Proces initial (nr. %d,%d) PID: %d, Parent PID: %d.\n", i1, i2, getpid(), getppid() );

    /* Bucla de producere a celor N procese fii. */
    for(i = 1; i < 6; i++)
    {
        i1=2; i2=i;
        if(-1 == (pid=fork()) )
        {
            perror("Eroare la fork");
            exit(2);
        }
        if(0 == pid)
        {
            wait(&codterm);
            printf("Proces fiu (nr. %d,%d), PID: %d, Parent PID: %d. Exit code: %d\n", i1, i2, getpid(), getppid(), codterm>>8);
            for(j = 1; j < 3; j++)
            {
                int i1, i2;
                i1=3; i2=(2*i)-2 + j;
                if (-1 == (pid2=fork()))
                {
                    perror("Eroare la fork");
                    exit(2);
                }
                if(0 == pid2)
                {
                    wait(&codterm);
                    printf("Proces fiu (nr. %d,%d), PID: %d, Parent PID: %d. Exit code: %d\n", i1, i2, getpid(), getppid(), codterm );
                    return j;
                }
            }
            

            for(j = 1; j < 3; j++)  // Procesul inițial va aștepta terminarea celor N fii; altfel, câteodată (i.e., nu neapărat la toate execuțiile)
            wait(NULL);         // este posibil ca unii fii să apară ca fiind orfani (i.e., având drept părinte procesul cu PID-ul 1).

            return i;  // Important: fiul nu trebuie să reia execuția buclei for, ca să nu creeze la rândul lui alte procese fii !
        }
    }

    /* Așteptarea terminării tuturor celor N fii. */
    for(i = 1; i < 6; i++)  // Procesul inițial va aștepta terminarea celor N fii; altfel, câteodată (i.e., nu neapărat la toate execuțiile)
        wait(NULL);         // este posibil ca unii fii să apară ca fiind orfani (i.e., având drept părinte procesul cu PID-ul 1).

    return 0;
}