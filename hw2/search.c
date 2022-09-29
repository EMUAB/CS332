#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MIN_DEPTH 0

struct stat stat_file;

char *filetype(unsigned char type) {
    char *str;
    switch (type) {
        case DT_BLK:
            str = "block device";
            break;
        case DT_CHR:
            str = "character device";
            break;
        case DT_DIR:
            str = "directory";
            break;
        case DT_FIFO:
            str = "named pipe (FIFO)";
            break;
        case DT_LNK:
            str = "symbolic link";
            break;
        case DT_REG:
            str = "regular file";
            break;
        case DT_SOCK:
            str = "UNIX domain socket";
            break;
        case DT_UNKNOWN:
            str = "unknown file type";
            break;
        default:
            str = "UNKNOWN";
    }
    return str;
}

/*TODO:
    -S  shows size, permissions, and last access time (0 size for
        directories)
   -s   list all files in heirarchy w/ size <= given
   -f   1) the file name contains the substring in the string pattern option
        2) the depth of the
        file relative to the starting directory of the traversal is less than
        or equal to the depth option (the starting directory itself has a depth
        of 0)

*/

int isANumber(char *input) {
    int i, len = strlen(input);
    for (i = 0; i < len; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

int recursiveDirs(char *parentDirName, int depth, int verbose) {
    DIR *parentDir;
    struct dirent *dirent;
    char time_buf[100];
    char parentDup[300];
    char newDir[300];

    // Set a duplicate of the dirname so it can be messed with
    strcpy(parentDup, parentDirName);

    parentDir = opendir(parentDirName);
    if (parentDir == NULL) {
        printf("Error opening directory '%s'\n", parentDirName);
        exit(-1);
    }

    while ((dirent = readdir(parentDir)) != NULL) {
        if (!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, ".."))
            continue;

        // Tabs over the entry per search level
        int i;
        for (i = 0; i < depth; i++) {
            printf("\t");
        }
        // Printing for -S flag
        if (verbose &&
            stat(strcat(parentDup, dirent->d_name), &stat_file) == 0) {
            strftime(time_buf, 100, "%x %I:%M%p",
                     localtime(&((&stat_file)->st_atim.tv_sec)));
            printf("%s \tSize: %ldb \tLast Accessed: %s\n", dirent->d_name,
                   (strcmp(filetype(dirent->d_type), "directory")
                        ? (&stat_file)->st_size
                        : 0),
                   time_buf);

            // Printing normally
        } else {
            printf("%s\n", dirent->d_name);
        }
        // Resets parentDup
        strcpy(parentDup, parentDirName);

        if (!strcmp(filetype(dirent->d_type), "directory") &&
            !(!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, ".."))) {
            strcpy(newDir, strcat(parentDup, strcat(dirent->d_name, "/")));
            strcpy(parentDup, parentDirName);

            recursiveDirs(newDir, depth + 1, verbose);
        }
    }
    closedir(parentDir);
    return 1;
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    long size = 0;
    char str_find[100];
    str_find[0] = '\0';

    int opt;
    char dir_name[200];

    // Sets the options
    if (argc != 1) {
        while ((opt = getopt(argc, argv, "Ss:f:")) != -1) {
            switch (opt) {
                // -S flag
                case 'S':
                    verbose = 1;
                    break;
                // -s flag
                case 's':
                    if (isANumber(optarg)) {
                        size = atol(optarg);
                    } else {
                        printf("Option -s only accepts a numerical value\n");
                        exit(-1);
                    }
                    break;
                // -f flag
                case 'f':
                    strcpy(str_find, optarg);
                    break;
                // Unknown flag or missing argument
                case '?':
                    exit(-1);
            }
        }
    }

    // Set the directory name
    if (argv[optind] != NULL || argv[optind] != 0x0) {
        int dir_name_len = strlen(argv[optind]);
        strcpy(dir_name, argv[optind]);
        // Enforces that the directory ends in a '/'
        if (dir_name[dir_name_len - 1] != '/') {
            dir_name[dir_name_len] = '/';
            dir_name[dir_name_len + 1] = '\0';
        }
    } else {
        strcpy(dir_name, "./");
    }

    recursiveDirs(dir_name, MIN_DEPTH, verbose);

    return 0;
}