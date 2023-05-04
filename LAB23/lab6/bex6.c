#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

unsigned int getPermissions(char who, char what) {
    unsigned int permission = 0;
    who += what;
    switch (who) {
        case 'g' + 'w':
            return S_IRGRP;
        case 'g' + 'r':
            return S_IWGRP;
        case 'g' + 'e':
            return S_IXGRP;
        case 'o' + 'w':
            return S_IRUSR;
        case 'o' + 'r':
            return S_IWUSR;
        case 'o' + 'e':
            return S_IXUSR;
        case 'a' + 'w':
            return S_IROTH;
        case 'a' + 'r':
            return S_IWOTH;
        case 'a' + 'e':
            return S_IXOTH;
    }
}

int main(int argc, char *argv[]) {
    if (argc == 0) {
        printf("Filename required\n");
        exit(1);
    }

    for (int file_id = 1; file_id < argc; file_id++) {
        const char * file_name = argv[file_id];
        char who;
        char what;
        printf("For whom do you want to change permission of file %s?\n", file_name);
        printf("1 - The file's group\n");
        printf("2 - The file's owner\n");
        printf("3 - Users other than the file's owner\n");
        char answer;
        scanf("%c", &answer);
        switch (answer) {
            case 1:
                who = 'g';
                break;
            case 2:
                who = 'o';
                break;
            case 3:
                who = 'a';
                break;
            default:
                who = 'o';
                break;
        }
        printf("What permission do you want to grant?\n");
        printf("1 - read\n");
        printf("2 - write\n");
        printf("3 - execute\n");
        scanf("%c", &answer);
        switch (answer) {
            case 1:
                who = 'r';
                break;
            case 2:
                who = 'w';
                break;
            case 3:
                who = 'e';
                break;
            default:
                who = 'e';
                break;
        }
        scanf("%c", &answer);
        int permission = chmod(file_name, getPermissions(who, what));
        if (permission != 0) {
            printf("Error reading '%s'\n", file_name);
            exit(1);
        }
    }

    return EXIT_SUCCESS;
}