#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MIN_DEPTH 0
#define BUF_SIZE 512

typedef void PRINTFUN(struct dirent *entry, char *dir_name);
char *e_command;

struct stat stat_file;

/**
 * @brief Determines the filetype of a given file and reduces it down to a
 * character. Based on the example from lab 5.
 *
 * @param type
 * @return char
 */
char filetype(unsigned char type)
{
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
int isNumber(char *input)
{
    int i, len = strlen(input);
    for (i = 0; i < len; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Given a file's perms, it prints them out directly
 *
 * @param perms
 */
void printPerms(mode_t perms)
{
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

/**
 * @brief Simple PRINTFUN
 *
 * @param entry
 * @param dir_name
 */
void simple(struct dirent *entry, char *dir_name)
{
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

/**
 * @brief Verbose PRINTFUN
 *
 * @param entry
 * @param dir_name
 */
void verbose(struct dirent *entry, char *dir_name)
{
    char time_buf[BUF_SIZE];

    simple(entry, dir_name);

    strftime(time_buf, BUF_SIZE, "%x %I:%M%p",
             localtime(&((&stat_file)->st_atim.tv_sec)));
    printf("\t(%ldb\t", (&stat_file)->st_size);
    printPerms((&stat_file)->st_mode);
    printf("\t%s)", time_buf);
}

void printEntry(struct dirent *entry, char *dir_name, PRINTFUN *fun)
{
    fun(entry, dir_name);
    printf("\n");
}

/**
 * @brief Runs exec once on the given file (Largely taken from lab7)
 *
 * @param entry
 * @param dir_name
 * @param e_flag
 */
void fork_each(struct dirent *entry, char *dir_name)
{
    char line[BUF_SIZE];
    char *total_args[BUF_SIZE];
    char *token;
    int count, pid, status;

    sprintf(line, "%s %s%s", e_command, dir_name, entry->d_name);

    // Make total args have an entry for each part of command
    count = 0;
    token = strtok(line, " ");
    while (token != NULL) {
        while (strstr(token, "\n") != NULL)
            token[strcspn(token, "\n")] = 0;
        total_args[count++] = token;
        token = strtok(NULL, " ");
    }
    total_args[count] = 0;

    pid = fork();
    if (pid == 0) {  // Child process exec
        execvp(total_args[0], total_args);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {  // Parent process
        wait(&status);
    }
    else {  // If fork() fails
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    // Final cleanup
    free(token);
}

/**
 * @brief Runs exec once with all of the given files
 *
 * @param files
 * @param size
 */
void fork_once(char *files)
{
    char line[BUF_SIZE];
    char *total_args[50];
    char *token;
    int count, pid, status;

    sprintf(line, "%s %s", e_command, files);

    // Make total args have an entry for each part of command
    count = 0;
    token = strtok(line, " ");
    while (token != NULL) {
        while (strstr(token, "\n") != NULL)
            token[strcspn(token, "\n")] = 0;
        total_args[count++] = token;
        token = strtok(NULL, " ");
    }
    total_args[count] = 0;

    pid = fork();
    if (pid == 0) {  // Child process exec
        execvp(total_args[0], total_args);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {  // Parent process
        wait(&status);
    }
    else {  // If fork() fails
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    // Final cleanup
    free(token);
}

void searchDir(char *dir_name, int depth, int S_flag, off_t max_size,
               int max_depth, char *f_substr, char t_flag, char e_flag)
{
    DIR *current_dir;
    struct dirent *entry;
    char dir_name_dup[BUF_SIZE];
    char new_dir[BUF_SIZE];
    char e_flag_files[BUF_SIZE * BUF_SIZE] = {0};

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
        int matches = f_substr[0] != '\0' && strstr(entry->d_name, f_substr) == NULL;

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
        if ((t_flag == 0 || t_flag == filetype(entry->d_type)) &&
            (!matches || f_substr[0] == '\0')) {
            if (e_flag == 'e') {
                fork_each(entry, dir_name_dup);
            }
            else if (e_flag == 'E') {
                int files_len = strlen(e_flag_files);
                char files_dup[files_len + 1];
                strcpy(files_dup, e_flag_files);
                sprintf(e_flag_files, "%s %s%s", files_dup, dir_name_dup, strdup(entry->d_name));
            }
            else if (S_flag && success_stat == 0) {
                printEntry(entry, dir_name_dup, verbose);
            }
            else {
                printEntry(entry, dir_name_dup, simple);
            }
        }

        // If directory is found, searchDir on that directory
        if (filetype(entry->d_type) == 'd') {
            strcpy(new_dir, strcat(dir_name_dup, strcat(entry->d_name, "/")));
            strcpy(dir_name_dup, dir_name);
            if (depth < max_depth || max_depth == -1)
                searchDir(new_dir, depth + 1, S_flag, max_size, max_depth,
                          f_substr, t_flag, e_flag);
        }
    }
    if (e_flag == 'E') fork_once(e_flag_files);
    closedir(current_dir);
}

int main(int argc, char *argv[])
{
    int verbose = 0;
    off_t max_size = LONG_MAX;
    char f_substr[BUF_SIZE];
    f_substr[0] = '\0';
    char t_flag = 0;
    char e_flag = 0;
    int max_depth = -1;

    int opt;
    char dir_name[200];

    // Sets the options
    if (argc != 1) {
        while ((opt = getopt(argc, argv, ":Ss:f:t:e:E:")) != -1) {
            switch (opt) {
                // -S flag
                case 'S':
                    verbose = 1;
                    break;
                // -s flag
                case 's':
                    if (isNumber(optarg)) {
                        max_size = atol(optarg);
                    }
                    else {
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
                case 'e':
                    e_flag = 'e';
                    e_command = optarg;
                    break;
                case 'E':
                    e_flag = 'E';
                    e_command = optarg;
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
                        case 'E':
                        case 'e':
                            printf("\"<unix-command with args>\"");
                            break;
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
    }
    else {
        strcpy(dir_name, "./");
    }
    searchDir(dir_name, MIN_DEPTH, verbose, max_size, max_depth, f_substr,
              t_flag, e_flag);
    return 0;
}