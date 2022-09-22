/* Sample program to illustrate C standard library usage */
/* This program write the stat structure for the file/directory 
 * provided by the command-line argument. The first command-line 
 * argument is the file/directory name and the second argument is 
 * the name of the output file. */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void printstat(struct stat statbuf);

int main(int argc, char** argv) {
	int fd;
	struct stat statbuf;

	if (lstat(argv[1], &statbuf) < 0) {
		printf("Error reading file/directory %s\n", argv[1]);
		perror("lstat");
		exit(-1);
	}
	printstat(statbuf);

	if ((fd = open(argv[2], O_CREAT | O_WRONLY, 0755)) == -1) {
		printf("Error opening file %s\n", argv[2]);
		perror("open");
		exit(-1);
	}
	write(fd, &statbuf, sizeof(struct stat));
	close(fd);

	return 0;
}

