#include <stdio.h> 
#include <stdlib.h>
#include <dirent.h> 
#include <string.h>

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

int recursiveDirs(char *parentDirName, int searchLevel) {
    DIR *parentDir;
    struct dirent *dirent;
    int count = 1;

    parentDir = opendir(parentDirName);
    if (parentDir == NULL) {
        printf("Error opening directory '%s'\n", parentDirName);
        exit(-1);
    }
    while ((dirent = readdir(parentDir)) != NULL) {
        int i;
        long int cdir = telldir(parentDir);
        for(i = 0; i < searchLevel; i++) {
          if (i == searchLevel-1) {
            if (readdir(parentDir) == NULL) {
              printf(" └──────");
            }
            else {
              printf(" ├──────");
            }
            seekdir(parentDir, cdir);
          }
          else {
            printf(" │");
            printf("\t");
          }
        }

        printf("[%d] %s (%s)\n", count, dirent->d_name, filetype(dirent->d_type));
        if (!strcmp(filetype(dirent->d_type), "directory") &&
            !(!strcmp(dirent->d_name, ".") || !strcmp(dirent->d_name, ".."))) {
            char parentDup[300];
            strcpy(parentDup, parentDirName);
            char newDir[300];
            strcpy(newDir, strcat(parentDup, strcat(dirent->d_name, "/")));
            recursiveDirs(newDir, searchLevel+1);
        }
        count++;
    }
    closedir(parentDir);
    return 1;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }
    recursiveDirs(argv[1], 0);
    return 0;
}