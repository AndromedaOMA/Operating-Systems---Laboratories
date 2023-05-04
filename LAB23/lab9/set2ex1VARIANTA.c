#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#define handle_error(msg) \
    { perror(msg); exit(EXIT_FAILURE); }

int main(int argn, char argv[]){
    if(argn<4){
        printf("Usage: %s <char1> <char2> <location> <destination>\n", argv[0]);
        exit(1);
    }
    char to_search = argv[1][0];
    char to_replace = argv[2][0];
    char file[100];
    char destination[100];
    strcpy(file,argv[3]);
    if(argn==5)
        strcpy(destination,argv[4]);
    else if(argn==4)
        strcpy(destination,argv[3]);
    //printf("%s %s",destination,file);

    int fd;
    struct stat file_stat;
    int file_size;
    if( (fd = open(file, O_RDWR)) == -1)
        handle_error("Eroare la deschiderea fisierului de intrare");
    if( fstat(fd, &file_stat) == -1)
        handle_error("Eroare la dimensiunea fisierului");
    file_size = file_stat.st_size;

    char file_content;
    if((file_content= mmap(0, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
        handle_error("Fisierul nu poate fi mapat");

    //REPLACE ch
    int i;
    for(i=0; file_content[i]!='\0'; i++){
        if(file_content[i]==to_search){
            file_content[i]=to_replace;
        }
    }

    //Daca este acelasi fisier, sincronizeaza schimbarile cu msync
    if(strcmp(destination,file)==0){
        if( -1 == msync(file_content, file_size, MS_SYNC))
            handle_error("Cannot sync the file");
        close(fd);
    }
    else{
        close(fd);
        if( (fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
            handle_error("Cannot open file or I don't have permissions");
        if( -1 == write(fd, file_content, file_size))
            handle_error("Cannot write the file");
        close(fd);
    }
    return 0;
}