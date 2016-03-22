#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
  int status1 = 0;
  struct stat filestat1;
  status1 = stat("file1.txt",&filestat1);

  printf("Return code %d\nFile size %d\n", status1, filestat1.st_size);

  int status2 = 0;
  struct stat filestat2;

  status2 = stat("test.c", &filestat2);

  printf("Return code %d\n File size %d\n", status2, filestat2.st_size);

  return 0;
}
