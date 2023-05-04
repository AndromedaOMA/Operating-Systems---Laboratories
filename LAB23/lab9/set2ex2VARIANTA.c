#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define handle_error(msg) \
    { perror(msg); exit(EXIT_FAILURE); }


int isNumber(char *s)
{
    if (!strchr("-0123456789", s[0]))
        return 0;
    for (int i = 1; s[i] != '\0'; i++)
        if (!strchr("0123456789", s[i]))
            return 0;
    return 1;
}


int main(int argc, char *argv[])
{
    int fd,c;
    int lines = 10;//DEFAULT
    int foundArg = 0;
    struct stat sb;
    size_t file_length;
    char *map_addr;
    char nume_fisier[255];
    

    if(argc == 1)
    {
        perror("Introdu un nume de fisier");
        exit(1);
    }
    if(argc == 2)
    {
        printf("Introduceti:\n-n pentru nr de linii \nsau \n-c pentru nr de biti \npe care doriti sa ii cititi din fisierul \"%s\"\n",argv[1]);
        exit(2);
    }
    else
    {
        strcpy(nume_fisier, argv[1]);
    }


    if (argc == 3 && strcmp("-n",argv[2]))
    {
        printf("Introdu numarul de biti");
        exit(3);
    }
    else
    {
        // step 2: deschiderea fișierului specificat.
        if (-1 == (fd = open(nume_fisier,O_RDONLY)) )
            handle_error("Eroare la deschiderea fisierului de date");


        // step 3: maparea fișierului în memorie.
        if (fstat(fd, &sb) == -1)   // pentru a obține dimensiunea fișierului, în octeți.
            handle_error("Error at fstat");
        file_length = sb.st_size;
        /* Apelul de mapare a întregului fișier specificat, în memorie. */
        map_addr = mmap( NULL,          // Se va crea o mapare începând de la o adresă page-aligned aleasă de kernel (și returnată în map_addr)
                         file_length,   // Lungimea mapării (de fapt, se alocă multiplu de pagini, dar restul din ultima pagină se umple cu zero-uri)
                         PROT_READ,     // Tipul de protecție a mapării: paginile mapării vor permite accese doar în citire
                         MAP_PRIVATE,   // Maparea este privată (doar pentru citirea fișierului, este suficient să fie privată)
                         fd,            // Descriptorul de fișier, din care se citesc octeții corespunzători porțiunii mapate în memorie
                         0              // Offset-ul, de la care începe porțiunea de fișier mapată în memorie, este 0, i.e. BOF
                       );
        if (map_addr == MAP_FAILED)
            handle_error("Error at mmap");



        fprintf(stderr, "Notification: A file mapping in memory was created, for the file: %s\n", nume_fisier);
        fprintf(stderr, "Notification: \tthe mapping corresponds to the file's content starting at offset: 0, and with the length: %ld\n", file_length);



        /* După crearea mapării, descriptorul de fișier poate fi închis imediat, fără a se invalida maparea ! */
        if (-1 == close(fd) )
            handle_error("Error at close");
        


        // Calculare numar de linii
        if (!strcmp("-n", argv[2]))
        {
            if(argc==4){
                if(!isNumber(argv[3]))
                {
                    perror("Introdu un numar");
                    exit(5);
                }
                else
                    lines = atoi(argv[3]);
            }
            foundArg = 1;
        }
        else
        {
            c = atoi(argv[3]);
            foundArg = 2;
        }
        
        // step 4: bucla de procesare -- se citesc primele 10 sau primele n linii din fisier
        printf("\n");
       
        if (foundArg == 1)//Avem pe -n
        {
            int lines_read = 0;
                for (int i = 0; i < file_length; i++)
                {
                    if (map_addr[i] == '\n')
                    {
                        lines_read++;
                        if (lines_read == lines)
                            break;
                    }
                   printf("%c",map_addr[i]);
                }
            printf("\n");
        }
        else if (foundArg == 2)//Avem pe -c
        {
            for (int i = 0; i < c; i++)
                   printf("%c",map_addr[i]);
             printf("\n");       
        }


        // step 5: "ștergem/distrugem" maparea creată anterior.(DEFAULT)
        if (-1 == munmap(map_addr, file_length) )
            handle_error("Error at munmap");

        exit(EXIT_SUCCESS);
    }
}