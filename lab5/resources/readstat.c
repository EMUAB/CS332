/* Sample program to illustrate C standard library usage */
/* Program reads in the stat structure from the input file specified
 * by the command-line argument and prints it to stdout */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

void printstat(struct stat sb);

int main(int argc, char** argv) {
	int fd;
	struct stat statbuf;

	if ((fd = open(argv[1], O_RDONLY)) == -1) {
		printf("Error opening file %s\n", argv[1]);
		perror("open");
		exit(-1);
	}
	read(fd, &statbuf, sizeof(struct stat));
	close(fd);

	printstat(statbuf);

	return 0;
}

