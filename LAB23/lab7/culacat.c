#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// VERSIUNE CU LACAT

void print_usage() {
    printf("Usage: ./faralacat ACCOUNTID (+/-)SUM\n");
    exit(EXIT_FAILURE);
}

int main (int argc, char *argv[]) {
    struct flock lock;
    if (argc-1 != 2) {
        printf("Numar invalid de argumente!\n");
        print_usage();
    }
    
    // DESCHIDE FISIERUL DATABASE.TXT
    int fd = open("database.txt", O_RDWR);
    if (fd == -1) {
        printf("ERROR: Nu se poate deschide fisierul!\n");
        exit(EXIT_FAILURE);
    }

    // PREGATIREA LACATULUI PENTRU FISIER
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    while (fcntl(fd, F_SETLKW, &lock) == -1) {
        printf("DEBUG: Se incearca punerea lacatului...");
        sleep(3);
    }

    // CAUTA ID-UL SPECIFICAT (LONG INT)
    long int specified_id = atoi(argv[1]);
    long int read_data;
    int read_sum;
    int read_operation;
    int id_is_found = 0;

    while (1) {
        read_operation = read(fd, &read_data, sizeof(long int));
        if (read_operation == -1) {
            printf("ERROR: Nu se poate citi din fisier!\n");
            exit(EXIT_FAILURE);
        }
        if (read_operation == 0) // FINAL DE FISIER
            break;
        printf("DEBUG:ID citit=%ld \n", read_data);
        if (read_data == specified_id) {
            id_is_found = 1;
            // CITESTE SUMA SPECIFICATA (INT)
            read_operation = read(fd, &read_sum, sizeof(int));
            if (read_operation == -1) {
            printf("ERROR: Nu se poate citi din fisier!\n");
                exit(EXIT_FAILURE);
            }
                printf("DEBUG:ID citit=%ld \n", read_data);
            break;
        }
        read(fd, &read_data, sizeof(int));
    }
    long int id = read_data;

    // NU PUTEM EXTRAGE O SUMA DE BANI DINT-UN CONT INEXISTENT
    int specified_sum = atoi(argv[2]);
    if (!id_is_found && specified_sum < 0) {
            printf("Nu poti extrage dintr-un cont inexistent!\n");
            exit(EXIT_FAILURE);
    }

    // DACA DEPOZITAM BANII INTR-UN CONT INEXISTENT, IL VOM CREA

    if (!id_is_found) {
        // SETAM STRUCTURA LACATULUI
        lock.l_type = F_UNLCK;
        lock.l_type =F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        while (fcntl(fd, F_SETLKW, &lock) == -1) {
        printf("DEBUG: Se incearca punerea lacatului...");
            sleep(3);
        }

        if(-1 == write(fd, &specified_id, sizeof(long int)) )
        {
            printf("ERROR: Nu se poate scrie in fisier!");
            exit(EXIT_FAILURE);
        }
        if(-1 == write(fd, &specified_sum, sizeof(int)) )
        {
            printf("ERROR: Nu se poate scrie in fisier!");
            exit(EXIT_FAILURE);
        }
        return EXIT_SUCCESS;
    }

    // CALCULAM NOUA SUMA
    int new_sum = read_sum + specified_sum;
    // SUMA E PREA MICA PENTRU EXTRAGERA SUMEI SPECIFICATE
    if (new_sum < 0) {
        printf("Fonduri insuficiente!\n");
        exit(EXIT_FAILURE);
    }
    // CAUTAM POZITIA CURSORULUI PENTRU SUPRASCRIEREA SUMEI
    close(fd);
    fd = open("database.txt", O_RDWR);
    if (fd == -1) {
        printf("ERROR: Cannot open file!\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        read_operation = read(fd, &read_data, sizeof(long int));
        if (read_data == id) {
            // trebuie sa veirific daca valoare s-a actualizat
            lock.l_type = F_UNLCK;
            lock.l_type =F_WRLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = 0;
            lock.l_len = 0;

            while (fcntl(fd, F_SETLKW, &lock) == -1) {
        printf("DEBUG: Se incearca punerea lacatului...");
                sleep(3);
            }

            write(fd, &new_sum, sizeof(int));
            printf("DEBUG:Noua suma=%d \n", new_sum);
            break;
        }
        read_operation = read(fd, &read_data, sizeof(int));
        
    }
    return 0;
}



