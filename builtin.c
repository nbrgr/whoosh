#include "builtin.h"
#include "whoosh.h"

void cd(char* path)
{
  int success = 0;
  if (path == NULL)
  {
    success = chdir(getenv("HOME"));
  }
  else{
    success = chdir(path);
  }
  if (success != 0)
  {
    printerror();
  }
}

void pwd()
{
  char* cwd = malloc(sizeof(char) * 256);
  cwd = getcwd(cwd, 256);
  printf("%s\n",cwd);
  free(cwd);
  return;
}

void path(char** paths)
{
  int pathcount = 0;
  int i = 0;
  while (paths[i] != NULL)
  {
    i++;
    pathcount++;
  }
  char** newpath = malloc(sizeof(char*) * (pathcount + 1));
  i = 0;
  while (paths[i] != NULL)
  {
    int length = strlen(paths[i]);
    newpath[i] = malloc(sizeof(char) * (length + 1));
    strncpy(newpath[i], paths[i], length + 1);
    i++;
  }
  newpath[i] == NULL;
  searchpath = newpath;
  return;
}

void shexit()
{
  exit(0);
}

void printerror()
{
  char error_message[30] = "An error has occurred\n";
  write(STDERR_FILENO, error_message, strlen(error_message));
  return;
}
