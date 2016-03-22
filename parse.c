#include "whoosh.h"

//Forward delcarations for functions copied from project 1:
int space(char c);

int nextword(char* string, int pos);

char** parseinput(char* input)
{
  int index = 0;
  int foundnewline = 0;
  while ((input[index] != '\0') && (!foundnewline))
  {
    if (input[index] == '\n')
    {
      foundnewline = 1;
      input[index] = '\0';
    }
    index++;
  }
  index = 0;
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
