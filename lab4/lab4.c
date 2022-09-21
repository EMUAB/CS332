#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFSIZE 4096

int main(int argc, char const* argv[]) {
    int rfd, wfd;
    long int n;
    char buf[BUFFSIZE];

    if (argc != 3) {
        printf("Usage: %s <destination> <source>\n", argv[0]);
        exit(-1);
    }

    wfd = open(argv[1], O_WRONLY);
    rfd = open(argv[2], O_RDONLY);

    if (rfd == -1 || wfd == -1) {
        printf("Error opening file\n");
        exit(-1);
    }
    if (lseek(wfd, 0, SEEK_END) >= 0) {
        while ((n = read(rfd, buf, BUFFSIZE)) > 0) {
            if (write(wfd, buf, n) != n) {
                printf("Error writing to %s", argv[1]);
            }
        }
    }
    if (n < 0) {
        printf("Error reading file\n");
        exit(-1);
    }

    close(wfd);
    close(rfd);
    return 0;
}
