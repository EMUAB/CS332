/* Sample C program to illustrate the use of opendir, readdir, and closedir
 * system calls. 
 * This version prints the file type next to the file name.
 */
#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 

char *filetype(unsigned char type) {
  char *str;
  switch(type) {
  case DT_BLK: str = "block device"; break;
  case DT_CHR: str = "character device"; break;
  case DT_DIR: str = "directory"; break;
  case DT_FIFO: str = "named pipe (FIFO)"; break;
  case DT_LNK: str = "symbolic link"; break;
  case DT_REG: str = "regular file"; break;
  case DT_SOCK: str = "UNIX domain socket"; break;
  case DT_UNKNOWN: str = "unknown file type"; break;
  default: str = "UNKNOWN";
  }
  return str;
}

int main (int argc, char **argv) { 
  struct dirent *dirent; 
  DIR *parentDir; 

  if (argc < 2) { 
    printf ("Usage: %s <dirname>\n", argv[0]); 
    exit(-1);
  } 
  parentDir = opendir (argv[1]); 
  if (parentDir == NULL) { 
    printf ("Error opening directory '%s'\n", argv[1]); 
    exit (-1);
  } 
  int count = 1; 
  while((dirent = readdir(parentDir)) != NULL){ 
    printf ("[%d] %s (%s)\n", count, dirent->d_name, filetype(dirent->d_type)); 
    count++; 
  } 
  closedir (parentDir); 
  return 0; 
} 

