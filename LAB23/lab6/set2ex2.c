#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define MAX_LINE_LENGTH 4096

void print_usage() {
    printf("Usage: head [-n num] [file ...]\n");
    exit(EXIT_FAILURE);
}

int calculateNrOfLinesToRead(int file_id, int fd, int nr_of_lines_to_read, char *argv[]) {
    char buffer[MAX_LINE_LENGTH];
    int total_lines = 0;

    while (1) {
        int bytes_read = read(fd, buffer, MAX_LINE_LENGTH);
        if (bytes_read == -1) {
            printf("Error reading file '%s'\n", argv[file_id]);
            break;
        } else if (bytes_read == 0) {
            break;
        }
        int lines_read = 0;
        for (int j = 0; j < bytes_read; j++) {
            if (buffer[j] == '\n') {
                lines_read++;
            }
        }
        total_lines += lines_read;
    }
    return total_lines + nr_of_lines_to_read; // nr_of_lines_to_read e deja negativ
}

int calculateNrOfBytesToRead(int file_id, int fd, int nr_of_bytes_to_read, char *argv[]) {
    char buffer[MAX_LINE_LENGTH];
    int total_bytes = 0;

    while (1) {
        int bytes_read = read(fd, buffer, MAX_LINE_LENGTH);
        if (bytes_read == -1) {
            printf("Error reading file '%s'\n", argv[file_id]);
            break;
        } else if (bytes_read == 0) {
            break;
        }
        total_bytes += bytes_read;
    }
    return total_bytes + nr_of_bytes_to_read; // nr_of_lines_to_read e deja negativ
}

void readLines(int file_id, int fd, int nr_of_lines_to_read, char *argv[]) {
    int total_lines_read = 0;
    char buffer[MAX_LINE_LENGTH];

    if (nr_of_lines_to_read < 0) {
        nr_of_lines_to_read = calculateNrOfLinesToRead(file_id, fd, nr_of_lines_to_read, argv);
        close(fd);
        int fd = open(argv[file_id], O_RDONLY);
    }

    while (total_lines_read < nr_of_lines_to_read) {
        int bytes_read = read(fd, buffer, MAX_LINE_LENGTH);
        if (bytes_read == -1) {
            printf("Error reading file '%s'\n", argv[file_id]);
            break;
        } else if (bytes_read == 0) {
            break;
        }
        
        int lines_read = 0;
        for (int j = 0; j < bytes_read; j++) {
            if (buffer[j] == '\n') {
                lines_read++;
                if (lines_read + total_lines_read == nr_of_lines_to_read) {
                    bytes_read = j + 1;
                    break;
                }
            }
        }
        
        int bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written == -1) {
            printf("Error writing to standard output\n");
            break;
        }
        
        total_lines_read += lines_read;
    }
}

void readBytes(int file_id, int fd, int nr_of_bytes_to_read, char *argv[]) {
    int total_bytes_read = 0;
    char buffer[MAX_LINE_LENGTH];

    if (nr_of_bytes_to_read < 0) {
        nr_of_bytes_to_read = calculateNrOfBytesToRead(file_id, fd, nr_of_bytes_to_read, argv);
        close(fd);
        int fd = open(argv[file_id], O_RDONLY);
    }

    while (total_bytes_read < nr_of_bytes_to_read) {
        int bytes_read = read(fd, buffer, MAX_LINE_LENGTH);
        if (bytes_read == -1) {
            printf("Error reading file '%s'\n", argv[file_id]);
            break;
        } else if (bytes_read == 0) {
            break;
        }
        
        for (int j = 0; j < bytes_read; j++) {
            if (j + total_bytes_read == nr_of_bytes_to_read) {
                bytes_read = j;
                break;
            }
        }
        
        int bytes_written = write(STDOUT_FILENO, buffer, bytes_read);
        if (bytes_written == -1) {
            printf("Error writing to standard output\n");
            break;
        }
        
        total_bytes_read += bytes_read;
    }
}
//
int main(int argc, char *argv[]) {
    int current_argument = 1;
    int nr_of_lines_to_read = 10; // Default to 10 lines
    int nr_of_bytes_to_read = 0;
    
    if (argc == 0) {
        print_usage();
        return 0;
    }

    // Check for flags
    if (strcmp(argv[current_argument], "-n") == 0) {
        nr_of_lines_to_read = atoi(argv[++current_argument]);
        if (nr_of_lines_to_read == 0) {
            print_usage();
            return 0;
        }
        // the first text file will be the 3'rd argument
        ++current_argument;
    } else if (strcmp(argv[current_argument], "-c") == 0) {
        nr_of_bytes_to_read = atoi(argv[++current_argument]);
        if (nr_of_lines_to_read == 0) {
            print_usage();
            return 0;
        }
        // the first text file will be the 3'rd/5'th argument
        ++current_argument;
    }
    
    
    // Process files
    for (int file_id = current_argument; file_id < argc; file_id++) {
        // OPEN FILE
        int fd = open(argv[file_id], O_RDONLY);
        if (fd == -1) {
            printf("Error opening file '%s'\n", argv[file_id]);
            continue;
        }
        
        printf("==> %s <==\n", argv[file_id]);
        
        if (nr_of_bytes_to_read) {
            readBytes(file_id, fd, nr_of_bytes_to_read, argv);
        }
        else {
            readLines(file_id, fd, nr_of_lines_to_read, argv);
        }
        if (close(fd) == -1) {
            printf("Error closing file '%s'\n", argv[file_id]);
            continue;
        }
    }
    
    return EXIT_SUCCESS;
}


