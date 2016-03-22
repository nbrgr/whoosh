#include "whoosh.h"

char* initialdir = "/bin";

int main(int argc, char** argv)
{
  int running = 1;
  //Initializing Our Path;
  searchpath = malloc(sizeof(char*) * 2);
  searchpath[0] = initialdir;
  searchpath[1] = NULL;
  char* command = malloc(sizeof(char) * 128);
  while (running)
  {
    printf("whoosh> ");
    char* success = fgets(command, 127, stdin);
    if (success == NULL)
    {
      printerror();
    }
    char** commandtokens = parseinput(command);
    execute(commandtokens);
  }
  return 0;
}
