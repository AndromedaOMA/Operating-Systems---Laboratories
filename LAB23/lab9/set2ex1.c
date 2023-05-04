#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/mman.h>


#define BLOCK_SIZE 4096
#define handle_error(msg) \
    { perror(msg); exit(EXIT_FAILURE); }
#define nullptr ((void*)0)


int main(int argc, char *argv[])
{
    char *map_addr, *map_addr2;
    int fd, fd2;
    struct stat sb, sb2;
    off_t offset, pa_offset;
    size_t length, adjusted_length;
    ssize_t codw;
    int flag_abort_prelucrare = 0;


    /* Determinăm dimensiunea paginilor corespunzătoare arhitecturii hardware a calculatorului dvs. */
    long PageSize = sysconf(_SC_PAGE_SIZE);
    if (PageSize == -1)
        handle_error("Error at sysconf");
    fprintf(stderr, "Notification: the page size on your hardware system is: %ld bytes.\n", PageSize);


    //Parametrii
    if (argc < 5)
    {
        printf("Usage: %s fisier_sursa fisier_destinatie char1 char2\n",argv[0]);
        exit(1);
    }


    fd = open(argv[1], O_RDWR);
    if (fd == -1)
        handle_error("Error at open");
    fd2 = open(argv[2], O_RDWR);
    if (fd2 == -1)
        handle_error("Error at open");
    if (fstat(fd, &sb) == -1)   /* To obtain file size */
        handle_error("Error at fstat");
    if (fstat(fd2, &sb2) == -1)   /* To obtain file size */
        handle_error("Error at fstat");
    offset = 0;
    fprintf(stderr, "Notification: the specified offset was: %ld\n", offset);


    if (offset >= sb.st_size)
    {
        fprintf(stderr, "Error: offset is past end of file\n");
        exit(EXIT_FAILURE);
    }
    
    
    pa_offset = offset & ~(PageSize - 1);   /* offset for mmap() must be page aligned */
    fprintf(stderr, "Notification: the page-aligned ajusted offset is: %ld\n", pa_offset);
    /* Aici, practic am calculat câtul întreg al împărțirii offset-ului la dimensiunea paginii, folosind operatori pe biți. */
    length = sb.st_size - offset;
    fprintf(stderr, "Notification: the corrected length (taking EOF into account) is: %ld\n", length);
    adjusted_length = length + offset - pa_offset;     


    /* Apelul de mapare a porțiunii de fișier specificate, în memorie. */
    map_addr = mmap( NULL,                   // Se va crea o mapare începând de la o adresă page-aligned aleasă de kernel (și returnată în map_addr)
                     adjusted_length,        // Lungimea aliniată (i.e. ajustată page-aligned) a mapării (calculată mai sus)
                     PROT_READ,              // Tipul de protecție a mapării: paginile mapării vor permite accese în citire și în scriere
                     MAP_SHARED,             // Maparea este partajată (altfel, ca mapare privată, nu se salvează nimic în fișierul de pe disc, la final)
                     fd,                     // Descriptorul de fișier, din care se citesc octeții corespunzători porțiunii mapate în memorie
                     pa_offset               // Offset-ul (deplasamentul) aliniat (i.e. ajustat page-aligned), de la care începe porțiunea de fișier mapată
                    );                       //   în memorie, i.e. porțiunea din fișier din care se citesc cei actual_length octeți, în maparea din memorie
    if (map_addr == MAP_FAILED)
        handle_error("Error at mmap");

    map_addr2 = mmap( NULL,                  // Se va crea o mapare începând de la o adresă page-aligned aleasă de kernel (și returnată în map_addr)
                     adjusted_length,        // Lungimea aliniată (i.e. ajustată page-aligned) a mapării (calculată mai sus)
                     PROT_READ | PROT_WRITE, // Tipul de protecție a mapării: paginile mapării vor permite accese în citire și în scriere
                     MAP_SHARED,             // Maparea este partajată (altfel, ca mapare privată, nu se salvează nimic în fișierul de pe disc, la final)
                     fd2,                    // Descriptorul de fișier, din care se citesc octeții corespunzători porțiunii mapate în memorie
                     pa_offset               // Offset-ul (deplasamentul) aliniat (i.e. ajustat page-aligned), de la care începe porțiunea de fișier mapată
                    );                       //   în memorie, i.e. porțiunea din fișier din care se citesc cei actual_length octeți, în maparea din memorie
    if (map_addr2 == MAP_FAILED)
        handle_error("Error at mmap");



    long mmap_size_in_pages = ( adjusted_length % PageSize ? 1 : 0 ) + ( adjusted_length / PageSize );
    fprintf(stderr, "Notification: \tthe length/size of the mapping in memory is: %ld page(s).\n", mmap_size_in_pages); 
    
    

    /* După crearea mapării, descriptorul de fișier poate fi închis imediat, fără a se invalida maparea ! */
    if (-1 == close(fd) )
        handle_error("Error at close");
    if (-1 == close(fd2) )
        handle_error("Error at close");



    /* Scriem pe ecran (stdout) conținutul mapării. Mai precis, afișăm doar zona mapată din fișier (fără începutul ei), nu întreaga mapare ! */
    /*
    printf("\nThe initial content of the mapping (read from the file), started from the original, non-aligned, offset, is the following:\n");
    codw = write(STDOUT_FILENO, map_addr + offset - pa_offset, length);
    if (codw == -1)
        handle_error("Error at 1st write on screen");
    if (codw != length) {
        fprintf(stderr, "Error at 1st write on screen: partial write completed.\n");
        exit(EXIT_FAILURE);
    }
    */

    printf("\n");

//----------------------------------------------------


    char raspuns;
    ///verificam daca putem accesa fisierul de iesire / tratam cazul din cerinta  *(y/n)
    if (access(argv[2], R_OK) == 0)
    {
        printf("Fisierul %s exista deja, doriti suprascrierea? (y/n)", argv[2]);
        do{
            scanf("%c", &raspuns);
        }while ((raspuns != 'y') && (raspuns != 'n'));
        if (raspuns == 'n')
            flag_abort_prelucrare = 1;
        else printf("Fisierul va fi suprascris!\n");
    }

    
    ///cazul in care suprascriem in fisierul de iesire
    if (!flag_abort_prelucrare)
    {
        char c1 = argv[3][0];
        char c2 = argv[4][0];
        int i;
        

        //printf("%s",map_addr2);
        //strncpy(map_addr2,"ABCDEFGHI_!",9);
        //for (i = 0; i < 9; i++) {  *(map_addr2 + i) = 'A'+i ; }
    

        for (i = 0; i < (int)length; i++) 
            if (*(map_addr + i) != c1)
                *(map_addr2 + i) = *(map_addr + i); 
            else
                *(map_addr2 + i) = c2;
                


        /* Ne asigurăm că modificările, operate în memorie asupra mapării, sunt salvate pe disc înainte de "ștergerea" mapării. */
        if (-1 == msync(map_addr2, adjusted_length, MS_SYNC) )
            handle_error("Error at msync");
        // Conform documentației, trebuie folosit apelul msync() pentru a fi siguri că administratorul memoriei din cadrul SO-ului a apucat să scrie pe disc
        // conținutul modificat al paginilor dirty din memorie, corespunzătoare acelei mapări, ÎNAINTE de a "șterge/distruge" acea mapare prin apelul munmap() !
    }
    else
    {	///cazul in care nu vrem sa suprascriem in fisierul de output
        fprintf(stderr, "Prelucrarea lui %s in %s nu s-a mai efectutat.\n", argv[1], argv[2]);
        return 2;
    }


    // "Ștergem/distrugem" maparea creată anterior.(DEFAULT)
    if (-1 == munmap(map_addr, adjusted_length))
        handle_error("Error at munmap");
    if (-1 == munmap(map_addr2, adjusted_length))
        handle_error("Error at munmap");
}




/*
struct stat {
               dev_t     st_dev;         // ID of device containing file 
               ino_t     st_ino;         // Inode number 
               mode_t    st_mode;        // File type and mode 
               nlink_t   st_nlink;       // Number of hard links 
               uid_t     st_uid;         // User ID of owner 
               gid_t     st_gid;         // Group ID of owner 
               dev_t     st_rdev;        // Device ID (if special file) 
               off_t     st_size;        // Total size, in bytes 
               blksize_t st_blksize;     // Block size for filesystem I/O 
               blkcnt_t  st_blocks;      // Number of 512B blocks allocated 

              

               struct timespec st_atim;  // Time of last access 
               struct timespec st_mtim;  // Time of last modification 
               struct timespec st_ctim;  // Time of last status change 

           #define st_atime st_atim.tv_sec      // Backward compatibility
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };

*/