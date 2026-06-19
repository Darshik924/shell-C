#include "hdr.h"
#include <limits.h>

// A function to flush our commands queue
void cleanCmds(char *cmd, int *x)
{
  while ((*x)-- > 0)
    *cmd++ = '\0';
  *x = 0;
}

// A function to remove specifically trailing spaces
void trimDown(char *str)
{
  if (str == NULL || *str == '\0') return;

  while (*str != '\0')
    ++str;
  while (*--str == ' ') 
    *str = '\0';
}

// Function that returns true if a command is builtin for this shell and false otherwise
bool isBuiltin(char *cmd)
{
  return (
    strncmp(cmd, echoCmd, 4) == 0 || 
    strncmp(cmd, typeCmd, 4) == 0 || 
    strncmp(cmd, exitCmd, 4) == 0
  );
}

// Function that returns a pointer to the first occurence of a character c and returns NULL otherwise
char *strChr(char *name, int c) 
{
  while (*name != c) {
    ++name;
    if (*name == c) 
      return name;

    if (*name == '\0') 
      return NULL; 
  } 
  return NULL;
} 

void freeArgv(char **argv) 
{
  for (int i=0; argv[i]; ++i) 
    free(argv[i]);

  free(argv);
}

void trimUp(char *cmd)
{ 
  trimDown(cmd);
  if (cmd == NULL || *cmd == '\0') return;

  while (*cmd == ' ')
    *cmd++ = '\0';
}

bool isEmpty(char *str)
{
  return (*str == '\0');    
}