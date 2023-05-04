#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>

char* map_addr;

void dialog_tata()
{
    char buf[100];
    FILE* ft;

    if(NULL == (ft = fopen("0.txt", "r")))
        perror("Eroare la open replici tata");

    while(!feof(ft))
    {
        fgets(buf, 100, ft);
        printf("[Father] %s\n", buf);
        if(!feof(ft) && *((int*)map_addr) == 0)
            *((int*)map_addr) = 1;
        //printf("E randul tau fiule!\n");
        //printf("Astept randul meu!\n");
        while(*((int*)map_addr) != 0)
            if(*((int*)map_addr) == 2)
                break;
    }
    *((int*)map_addr) = 2;
}

void dialog_fiu()
{
    char buf2[100];
    FILE* ff;
    if(NULL == (ff = fopen("1.txt", "r")))
        perror("Eroare la open replici fiu");
        
    while(!feof(ff)){
        //printf("Astept randul meu!\n");
        while(*((int*)map_addr) != 1)
            if(*(int*)map_addr == 2)
                break;
                
    fgets(buf2, 100, ff);
    printf("[Son] %s\n", buf2);
    if(!feof(ff) && *((int*)map_addr) == 1)
        *((int*)map_addr) = 0;
        //printf("E randul tau tata!\n");
    }
    *((int*)map_addr) = 2;
}

int main()
{
	pid_t pid_fiu;
    int fd;
    if (-1 == (fd = open("flag.bin",O_RDWR | O_CREAT, 0666)))
        perror("Eroare la deschiderea fisierului de date");
    ftruncate(fd, 4);
    if((map_addr = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))==MAP_FAILED){
        perror("Eroare la mmap");
    }

    //Set the flag to 0
    *((int*)map_addr) = 0;
    if(-1 == (pid_fiu=fork()) ){
        perror("Eroare la fork");
    }

	/* Ramificarea execuției în cele două procese, tată și fiu. */
	if(pid_fiu == 0)
	{   /* Zona de cod executată doar de către fiu. */

		printf("\n[P1] Procesul fiu, cu PID-ul: %d.\n", getpid());
		dialog_fiu();
	}
	else
	{   /* Zona de cod executată doar de către părinte. */

		printf("\n[P0] Procesul tata, cu PID-ul: %d.\n", getpid());
		dialog_tata();
	}

	/* Zona de cod comună, executată de către ambele procese */
	//printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );
	return 0;
}