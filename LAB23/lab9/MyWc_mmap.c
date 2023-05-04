#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    int fd, i;
    char *addr, *p;
    struct stat sb;
    off_t offset = 0;
    size_t size;
    int num_chars = 0, num_words = 0, num_lines = 0, max_line_len = 0;
    int in_word = 0, line_len = 0;
    char buffer[BUFFER_SIZE];

    if (argc != 2)
    {
        fprintf(stderr, "Scrieti %s <nume_fisier>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &sb) == -1)
    {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    size = sb.st_size;

    addr = mmap(NULL, 
                size, 
                PROT_READ, 
                MAP_PRIVATE, 
                fd, 
                0);
    if (addr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    p = addr;

    while (offset < size) {
        if (offset + BUFFER_SIZE > size)
        {
            size_t remaining = size - offset;
            memcpy(buffer, p, remaining);
            p += remaining;
            offset += remaining;
        } 
        else
        {
            memcpy(buffer, p, BUFFER_SIZE);
            p += BUFFER_SIZE;
            offset += BUFFER_SIZE;
        }

        for (i = 0; i < BUFFER_SIZE && offset + i < size; i++)
        {
            num_chars++;

            if (buffer[i] == '\n')
            {
                num_lines++;
                line_len = 0;
            } 
            else
                line_len++;

            if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t')
                in_word = 0;
            else 
                if (in_word == 0)
                {
                    in_word = 1;
                    num_words++;
                }

            if (line_len > max_line_len)
                max_line_len = line_len;
        }
    }

    printf("%d %d %d %d\n", num_lines, num_words, num_chars, max_line_len);

    if (munmap(addr, size) == -1)
    {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1)
    {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}
