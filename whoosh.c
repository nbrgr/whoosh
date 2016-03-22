#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Forward Declarations

void printerror();

char** parseinput(char* input);
int space(char c);
int nextword(char* string, int pos);

void execute(char** args);
char* getprogrampath(char* program);

void cd(char* path);
void pwd();
void path(char** paths);
void shexit();
void printerror();

void prompt();

//Global Variables

char* initialdir = "/bin";
char** searchpath;

int main(int argc, char** argv)
{
  if (argc != 1)
  {
    printerror();
    exit(1);
  }
  int running = 1;
  //Initializing Our Path;
  searchpath = malloc(sizeof(char*) * 2);
  searchpath[0] = initialdir;
  searchpath[1] = NULL;
  char* command = malloc(sizeof(char) * 1024);
  while (running)
  {
    prompt();
    bzero(command, 1024);
    char* success = fgets(command, 1024, stdin);
    if (success != NULL)
    {
      char** commandtokens = parseinput(command);
      if (commandtokens != NULL)
      {
        execute(commandtokens);
      }
    }
  }
  free(command);
  return 0;
}

void prompt()
{
  printf("whoosh> ");
  fflush(stdout);
}

char** parseinput(char* input)
{
  int index = 0;
  int foundnewline = 0;
  int length = strlen(input);
  if (length > 129)
  {
    printerror();
    return NULL;
  }
  while ((input[index] != '\0') && (!foundnewline))
  {
    if (input[index] == '\n')
    {
      foundnewline = 1;
      input[index] = ' ';
    }
    index++;
  }
  index = 0;
  if (!foundnewline)
  {
    return NULL;
  }
  char separator[] = " ";
  int tokencount = 0;
  char** output = NULL;
  while(input[index] != '\0')
  {
    index = nextword(input, index);
    tokencount++;
  }
  output = malloc(sizeof(char*) * (tokencount + 1));
  index = 1;
  output[0] = strtok(input, " ");
  if (output[0] == NULL)
  {
    return NULL;
  }
  while(output[index] = strtok(NULL, " "))
  {
    index++;
  }
  return output;
}


//Funcions copied from project 1:
int space(char c)
{
  if ((int)c == 32)
    {
      return 1;
    }
  return 0;
}

int nextword(char* string, int pos)
{
  int poscopy = pos;
  while (string[pos] != '\0' && ! space(string[pos]))
    {
      pos++;
    }
  while (string[pos] != '\0' && space(string[pos]))
    {
      pos++;
    }
  return pos;
}

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

void execute(char** args)
{
  if (args == NULL)
  {
    return;
  }
  if (strcmp(args[0], "cd") == 0)
  {
    cd(args[1]);
  }
  else if (strcmp(args[0], "path") == 0)
  {
    path(&(args[1]));
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
      if ((strcmp(args[i], ">") == 0))
      {
        if (args[i + 1] == NULL)
        {
          printerror();
          return;
        }
        redirect = args[i + 1];
        args[i] = NULL;
        if (args[i + 2] != NULL)
        {
          printerror();
          return;
        }
      }
      i++;
    }

    args[0] = programpath;

    int pid = fork();

    if (pid < 0)
    {
      printerror();
      return;
    }
    else if (pid == 0)
    {
      int index = 0;

      if (redirect != NULL)
      {
        char* out = malloc(sizeof(char) * 64);
        char* err = malloc(sizeof(char) * 64);
        int errstatus = 0;
        int outstatus = 0;
        bzero(out, 64);
        bzero(err, 64);
        strncpy(out, redirect, 64);
        strncpy(err, redirect, 64);
        strncat(out, ".out", 64);
        strncat(err, ".err", 64);
        close(STDOUT_FILENO);
        open(out, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        close(STDERR_FILENO);
        open(err, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
        free(out);
        free(err);
        if (errstatus < 0 || outstatus < 0)
        {
          printerror();
          exit(1);
        }
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
