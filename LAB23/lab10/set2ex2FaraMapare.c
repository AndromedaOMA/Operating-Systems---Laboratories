#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


char CitireFlag()
{
	FILE * fp;

    fp = fopen("flag.bin", "r");
    if (fp == NULL)
        exit(1);

	char flag;
	flag=fgetc(fp);
	fclose(fp);

	// printf("%c\n",flag);
			
	return flag;
}

void ScriereFlag(char flag)
{
	FILE * fp;

    fp = fopen("flag.bin", "w");
    if (fp == NULL)
        exit(1);

	fputc(flag,fp);
	fclose(fp);
}


void dialog_tata()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("0.txt", "r");
    if (fp == NULL)
        exit(1);

    while ((read = getline(&line, &len, fp)) != -1)
        {			
			printf("%s", line);
			char flag=CitireFlag();
			if(flag=='0')
			ScriereFlag('1');

			flag=CitireFlag();
			while(flag!='0')
			{
				if(flag=='2')
					break;
				flag=CitireFlag();
			}
		}
	printf("GATA: parinte");
	ScriereFlag('2');

    fclose(fp);
    if (line)
        free(line);
    wait(0);
}

void dialog_fiu()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("1.txt", "r");
    if (fp == NULL)
        exit(1);

    while ((read = getline(&line, &len, fp)) != -1)
        {
			char flag=CitireFlag();
			while(flag!='1')
			{
				if(flag=='2')
					break;
				flag=CitireFlag();
			}
			printf("%s", line);
			if(flag=='1')
				ScriereFlag('0');
		}
	printf("GATA: copil");
	ScriereFlag('2');


    fclose(fp);
    if (line)
        free(line);
    exit(0);
}

int main()
{
	pid_t pid_fiu;

	/* Crearea unui proces fiu. */
	if(-1 == (pid_fiu=fork()) )
	{
		perror("Eroare la fork");  return 1;
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
	printf("Sfarsitul executiei procesului %s.\n\n", pid_fiu == 0 ? "fiu" : "parinte" );

	return 0;
}