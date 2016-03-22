#include "execute.h"
#include "whoosh.h"

void execute(char** args)
{
  if (strcmp(args[0], "cd") == 0)
  {
    cd(args[1]);
  }
  else if (strcmp(args[0], "path") == 0)
  {
    path(args+1);
  }
  else if (strcmp(args[0], "exit") == 0)
  {
    shexit();
  }
  else if (strcmp(args[0], "pwd") == 0)
  {
    pwd();
  }
  else
  {
    char* programpath = getprogrampath(args[0]);
    if (programpath == NULL)
    {
      printerror();
      return;
    }
    int i = 0;
    char* redirect = NULL;
    while (args[i] != NULL)
    {
      if ((strcmp(args[i], ">") == 0) && (args[i + 1] != NULL))
      {
        redirect = args[i + 1];
        args[i] = NULL;
      }
      i++;
    }

    args[0] = programpath;

    int pid = fork();

    if (pid < 0)
    {
      //handle error
    }
    else if (pid == 0)
    {
      int index = 0;

      if (redirect != NULL)
      {
        //handle redirection
      }
      execv(programpath, args);
    }
    else
    {
      int status = 0;
      waitpid(pid, &status, 0);
    }
  }
}

char* getprogrampath(char* program)
{
  int index = 0;
  int found = 0;
  struct stat exists;
  char* testpath = malloc(sizeof(char) * 256);
  while ((searchpath[index] != NULL) && (!found))
  {
    bzero(testpath, 256);
    strncpy(testpath, searchpath[index], 256);
    strncat(testpath, "/", 256);
    strncat(testpath, program, 256);
    int status = stat(testpath, &exists);
    if (status == 0)
    {
      found = 1;
    }
    index++;
  }

  if (found == 0)
  {
    free(testpath);
    return NULL;
  }

  return testpath;
}
