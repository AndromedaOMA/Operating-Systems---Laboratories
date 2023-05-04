#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define BLOCK_SIZE 4096

#define handle_error(msg) \
    { perror(msg); exit(EXIT_FAILURE); }

#define nullptr ((void*)0)

int main(int argc, char *argv[])
{
    char *map_addr;
    int fd;
    struct stat sb;
    off_t offset=0, pa_offset;
    size_t length, adjusted_length;
    ssize_t codw;

    int flagC = 0, flagN = 0, cntC = 0, cntN = 0;
    int i, fileNameIndex = 0, input, readStat, lines, bytes;
    char c;

    long PageSize = sysconf(_SC_PAGE_SIZE);
    if (PageSize == -1)
        handle_error("Error at sysconf");
    //fprintf(stderr, "Notification: the page size on your hardware system is: %ld bytes.\n", PageSize);

    if (argc < 2) 
    {
        printf("Usage: %s!\n", argv[0]);
        fprintf(stderr, "Usage: %s!\n", argv[0]);
        exit(1);
    }

    for (i = 1; i < argc; i++)
    {
        if (0 == strcmp(argv[i], "-c"))
        {
            if (flagC > 0 || fileNameIndex != 0)
            {
                printf("Invalid flag: c\n");
                fprintf(stderr, "Invalid flag: c\n");
                exit(2);
            }
            flagC = 1;
        }
        else if (0 == strcmp(argv[i], "-n"))
        {
            if (flagN > 0 || fileNameIndex != 0)
            {
                printf("Invalid flag: n\n");
                fprintf(stderr, "Invalid flag: n\n");
                exit(3);
            }
            flagN = 1;
        }
        else if ((argv[i][0] >= '0' && argv[i][0] <= '9') || (argv[i][0] == '-' && argv[i][0] >= '0' && argv[i][0] <= '9'))
        {
           if (flagC == 1)
           {
               cntC = atoi(argv[i]);
               flagC = 2;
           }
           else if (flagN == 1)
           {
               cntN = atoi(argv[i]);
               flagN = 2;
           }
           else
           {
               printf("Invalid numeric character!\n");
               fprintf(stderr, "Invalid numeric character!\n");
               exit(4);
           }
        }
        else
        {
            fileNameIndex = i;
            break;
        }
    }

    if (fileNameIndex == 0)
    {
        printf("No file specified\n");
        fprintf(stderr, "No file specified\n");
        exit(5);
    }

    if (flagC == 1 || flagN == 1)
    {
        printf("Wrong input\n");
        fprintf(stderr, "Wrong input\n");
        exit(6);
    }
    input = open(argv[i], O_RDONLY);
    if (fstat(input, &sb) == -1)   // pentru a obține dimensiunea fișierului, în octeți.
            handle_error("Error at fstat");

    pa_offset = offset & ~(PageSize - 1);   /* offset for mmap() must be page aligned */
    //fprintf(stderr, "Notification: the page-aligned ajusted offset is: %ld\n", pa_offset);
    /* Aici, practic am calculat câtul întreg al împărțirii offset-ului la dimensiunea paginii, folosind operatori pe biți. */
    length = sb.st_size - offset;
    //fprintf(stderr, "Notification: the corrected length (taking EOF into account) is: %ld\n", length);
    adjusted_length = length + offset - pa_offset;     
    
    
    /* Apelul de mapare a porțiunii de fișier specificate, în memorie. */
    map_addr = mmap( NULL,                   // Se va crea o mapare începând de la o adresă page-aligned aleasă de kernel (și returnată în map_addr)
                     adjusted_length,        // Lungimea aliniată (i.e. ajustată page-aligned) a mapării (calculată mai sus)
                     PROT_READ,              // Tipul de protecție a mapării: paginile mapării vor permite accese în citire și în scriere
                     MAP_PRIVATE,             // Maparea este partajată (altfel, ca mapare privată, nu se salvează nimic în fișierul de pe disc, la final)
                     input,                  // Descriptorul de fișier, din care se citesc octeții corespunzători porțiunii mapate în memorie
                     pa_offset               // Offset-ul (deplasamentul) aliniat (i.e. ajustat page-aligned), de la care începe porțiunea de fișier mapată
                    );                       // în memorie, i.e. porțiunea din fișier din care se citesc cei actual_length octeți, în maparea din memorie
    if (map_addr == MAP_FAILED)
        handle_error("Error at mmap");

    long mmap_size_in_pages = ( adjusted_length % PageSize ? 1 : 0 ) + ( adjusted_length / PageSize );
    //fprintf(stderr, "Notification: \tthe length/size of the mapping in memory is: %ld page(s).\n", mmap_size_in_pages); 
    
    /* După crearea mapării, descriptorul de fișier poate fi închis imediat, fără a se invalida maparea ! */
    if (-1 == close(input) )
        handle_error("Error at close");
    
    
    for (i = fileNameIndex; i < argc; i++)
    {
        lines = 1;
        bytes = 0;
    
        if (flagN > 0)
        {

            int lines_read = 0;
            for (int i = 0; i < length; i++)
            {
                if (map_addr[i] == '\n')
                {

                    lines_read++;
                    if (lines_read == cntN)
                        break;
                }
                printf("%c", map_addr[i]);
            }
            printf("\n");
        }

        if (flagC > 0)
        {

            for (int i = 0; i < cntC; i++)
            {
                printf("%c", map_addr[i]);
            }
            printf("\n");
        
        }
        printf("bytes: %d, lines: %d", bytes, lines);
    }
               

    // "Ștergem/distrugem" maparea creată anterior.(DEFAULT)
    if (-1 == munmap(map_addr, adjusted_length))
        handle_error("Error at munmap");
           
    return 0;
}