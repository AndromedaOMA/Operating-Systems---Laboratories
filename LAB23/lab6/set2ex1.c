#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define BLOCK_SIZE 4096

int main(int argc, char *argv[])
{
    int flag_abort_prelucrare = 0;
    ///verificam daca avem nr corect de parametrii
    if (argc < 5)
    {
        printf("Usage: %s fisier_sursa fisier_destinatie char1 char2\n",argv[0]);
        exit(1);
    }
    ///verificam daca putem accesa fisierul de intrare
    if (access(argv[1], R_OK) != 0)
    {
        fprintf(stderr, "Eroare: fisierul sursa %s nu este accesibil (i.e nu exista, sau nu am drept de citire)\t", argv[1]);
        perror("Cauza este");
        return 1;
    }
    char raspuns;
    ///verificam daca putem accesa fisierul de iesire / tratam cazul din cerinta
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
        int input_fd, output_fd;
        ssize_t bytes_in, bytes_out;
        char buffer[BLOCK_SIZE];
        ///verificam daca putem deschide fisierul de intrare
        if ( -1 == (input_fd = open(argv[1], O_RDONLY)))
        {
            fprintf(stderr, "Eroare la deschiderea fisierului sursa %s\n", argv[1]);
            perror("Cauza este");
            return 3;
        }
        ///verificam daca putem deschide fisierul de iesire
        if ( -1 == (output_fd = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC,0600)))
        {
            fprintf(stderr, "Eroare la deschiderea fisierului destinatie %s\n", argv[2]);
            perror("Cauza este");
            return 4;
        }
        /// in c1 si c2 stocam caracterele din input, cum ni se spune in cerinta
        char c1 = argv[3][0];
        char c2 = argv[4][0];
        while (1) /// copiem din input in output file si inlocuim unde este necesar
        {
            if ( -1 == (bytes_in = read(input_fd, buffer, BLOCK_SIZE)))
            {
                fprintf(stderr, "Eroare la citirea din fisierul %s\n", argv[1]);
                perror("Cauza este");
                return 5;
            }
            /// daca am ajuns la EOF in fisierul de intrare
            if (bytes_in == 0)
                break;
            int k = 0;
            char bufferOut[BLOCK_SIZE];
            for (int i = 0; i < bytes_in; i++)
                if (buffer[i] == c1) /// daca am gasit un caracter egal cu c1
                    bufferOut[k++] = c2; // in scriem in bufferOut cu c2
                else bufferOut[k++] = buffer[i]; /// altfel doar copiem caracterul
            if ( -1 == (bytes_out = write(output_fd, bufferOut, k)))
            {
                fprintf(stderr, "Eroare la scrierea in fisierul %s\n", argv[2]);
                perror("Cauza este");
                return 6;
            }
            if (bytes_out != k)
            {
                fprintf(stderr, "Fatal write error: scriere incompleta!\n");
                return 7;
            }
        }
        close(input_fd);
        close(output_fd);
        printf("Am prelucrat cu succes fisierul sursa %s in destinatia %s\n", argv[1], argv[2]);
        return 0;
    }
    else
    {	///cazul in care nu vrem sa suprascriem in fisierul de output
        fprintf(stderr, "Prelucrarea lui %s in %s nu s-a mai efectutat.\n", argv[1], argv[2]);
        return 2;
    }
}