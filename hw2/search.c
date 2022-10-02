#include <ctype.h>
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define MIN_DEPTH 0

typedef void PRINTFUN(struct dirent *entry, char *dir_name);

struct stat stat_file;

/**
 * @brief Determines the filetype of a given file and reduces it down to a
 * character. Based on the example from lab 5.
 *
 * @param type
 * @return char
 */
char filetype(unsigned char type) {
    char ch;
    switch (type) {
        case DT_BLK:
            ch = 'b';
            break;
        case DT_CHR:
            ch = 'c';
            break;
        case DT_DIR:
            ch = 'd';
            break;
        case DT_FIFO:
            ch = 'p';
            break;
        case DT_LNK:
            ch = 'l';
            break;
        case DT_REG:
            ch = 'f';
            break;
        case DT_SOCK:
            ch = 's';
            break;
        case DT_UNKNOWN:
            ch = 'u';
            break;
        default:
            ch = 'u';
    }
    return ch;
}

/**
 * @brief Checks if a given string strictly contains numerical digits
 *
 * @param input string
 * @return 0 - passes, 1 - fails
 */
int isNumber(char *input) {
    int i, len = strlen(input);
    for (i = 0; i < len; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

void printPerms(mode_t perms) {
    printf("%c", (perms & S_IRUSR) ? 'r' : '-');
    printf("%c", (perms & S_IWUSR) ? 'w' : '-');
    printf("%c", (perms & S_IXUSR) ? 'x' : '-');
    printf("%c", (perms & S_IRGRP) ? 'r' : '-');
    printf("%c", (perms & S_IWGRP) ? 'w' : '-');
    printf("%c", (perms & S_IXGRP) ? 'x' : '-');
    printf("%c", (perms & S_IROTH) ? 'r' : '-');
    printf("%c", (perms & S_IWOTH) ? 'w' : '-');
    printf("%c", (perms & S_IXOTH) ? 'x' : '-');
}

// Simple PRINTFUN
void simple(struct dirent *entry, char *dir_name) {
    char *buf;

    printf("%s ", entry->d_name);

    // Prints the name of the file referenced if symlink
    if (filetype(entry->d_type) == 'l') {
        ssize_t nbytes, bufsize;
        strcat(dir_name, entry->d_name);

        // The following is based off of the example here:
        // https://man7.org/linux/man-pages/man2/readlink.2.html
        bufsize = stat_file.st_size == 0 ? PATH_MAX : stat_file.st_size + 1;
        buf = malloc(bufsize);
        nbytes = readlink(dir_name, buf, bufsize);
        printf("(%.*s)", (int)nbytes, buf);
        free(buf);
    }
}

// Verbose PRINTFUN 
void verbose(struct dirent *entry, char *dir_name) {
    char time_buf[100];

    simple(entry, dir_name);

    strftime(time_buf, 100, "%x %I:%M%p",
             localtime(&((&stat_file)->st_atim.tv_sec)));
    printf("\t(%ldb\t", (&stat_file)->st_size);
    printPerms((&stat_file)->st_mode);
    printf("\t%s)", time_buf);
}

//
void printEntry(struct dirent *entry, char *dir_name, PRINTFUN *f) {
    f(entry, dir_name);
    printf("\n");
}

/**
 * @brief
 *
 * @param dir_name      Directory string name
 * @param depth         Current search depth
 * @param verbose       Is/not verbose
 * @param max_size      Specified max size
 * @param max_depth     Specified max depth
 * @param find_substring    Specified substring
 * @param t_flag        'f' or 'd' if -t is used
 */
void printDir(char *dir_name, int depth, int S_flag, off_t max_size,
              int max_depth, char *f_substr, char t_flag) {
    DIR *current_dir;
    struct dirent *entry;
    char dir_name_dup[300];
    char new_dir[300];

    // Set a duplicate of the dirname so it can be messed with
    strcpy(dir_name_dup, dir_name);

    // Open the directory
    current_dir = opendir(dir_name);
    if (current_dir == NULL) {
        printf("Error opening directory '%s'\n", dir_name);
        exit(-1);
    }

    // Main loop that prints each file
    while ((entry = readdir(current_dir)) != NULL) {
        // Skips the . and .. directory files
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // Sets the stat_file
        int success_stat =
            lstat(strcat(dir_name_dup, entry->d_name), &stat_file);
        strcpy(dir_name_dup, dir_name);

        // Sets the size of directories to 0
        if (filetype(entry->d_type) == 'd') {
            (&stat_file)->st_size = 0;
        }
        int matches =
            f_substr[0] != '\0' && strstr(entry->d_name, f_substr) == NULL;

        // Skips if the file's size is over max_size
        // OR if the substring doesn't match
        if (((&stat_file)->st_size > max_size)) {
            continue;
        }
        // Tabs over the entry per search level
        int i;
        if ((t_flag != 'd' || filetype(entry->d_type) == 'd') && !matches) {
            for (i = 0; i < depth; i++) {
                printf("\t");
            }
        }
        // Prints when there is no t flag, t flag is f and file is regular, t
        // flag is f and file is directory Does NOT print when the f flag exists
        // and there is not a match
        if ((t_flag == 0 || (t_flag == filetype(entry->d_type))) &&
            (!matches || f_substr[0] == '\0')) {
            // Printing for -S flag
            if (S_flag && success_stat == 0)
                printEntry(entry, dir_name_dup, verbose);
            else
                printEntry(entry, dir_name_dup, simple);
        }

        // If directory is found, printDir on that directory
        if (filetype(entry->d_type) == 'd') {
            strcpy(new_dir, strcat(dir_name_dup, strcat(entry->d_name, "/")));
            strcpy(dir_name_dup, dir_name);
            if (depth < max_depth || max_depth == -1)
                printDir(new_dir, depth + 1, S_flag, max_size, max_depth,
                         f_substr, t_flag);
        }
    }
    closedir(current_dir);
}

int main(int argc, char *argv[]) {
    int verbose = 0;
    off_t max_size = LONG_MAX;
    char f_substr[100];
    f_substr[0] = '\0';
    char t_flag = 0;
    int max_depth = -1;

    int opt;
    char dir_name[200];

    // Sets the options
    if (argc != 1) {
        while ((opt = getopt(argc, argv, ":Ss:f:t:")) != -1) {
            switch (opt) {
                // -S flag
                case 'S':
                    verbose = 1;
                    break;
                // -s flag
                case 's':
                    if (isNumber(optarg)) {
                        max_size = atol(optarg);
                    } else {
                        printf("Option -s only accepts a numerical value\n");
                        exit(-1);
                    }
                    break;
                // -f flag
                case 'f':
                    strcpy(f_substr, strtok(optarg, " "));
                    max_depth = atoi(strtok(NULL, " "));
                    break;
                // -t flag
                case 't':
                    t_flag = optarg[0];
                    break;
                // Missing argument
                case ':':
                    printf("Option -%c should have argument ", optopt);
                    switch (optopt) {
                        case 's':
                            printf("<size in bytes>");
                            break;
                        case 'f':
                            printf("\"<string pattern> <depth>\"");
                            break;
                        case 't':
                            printf("<option>");
                        default:
                            exit(-1);
                    }
                    printf("\n");
                    exit(-1);
                // Unknown flag
                case '?':
                    printf("Unknown option: %c\n", optopt);
                    exit(-1);
            }
        }
    }

    // Set the directory name
    if (argv[optind] != NULL) {
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
    printDir(dir_name, MIN_DEPTH, verbose, max_size, max_depth, f_substr,
             t_flag);
    return 0;
}