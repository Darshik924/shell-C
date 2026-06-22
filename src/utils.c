#include "hdr.h"
#include <limits.h>

// A function to flush our commands queue
void cleanCmds(char *cmd, int *x)
{
  while (*cmd != '\0')
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
    strncmp(cmd, exitCmd, 4) == 0 ||
    strncmp(cmd, pwdCmd, 3)
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

int strLen(char *str)
{
  int i = 0;
  if (str == NULL || *str == '\0') return 0;

  while (*str++ != '\0')
    ++i;
  return i;
}

void trimUp(char tmp[])
{ 
  char *cmd = tmp;
  trimDown(cmd);
  if (cmd == NULL || *cmd == '\0') return;
  int len = strLen(tmp);

  while (*cmd == ' ')
    ++cmd;
  int i = 0;
  while ((tmp[i++] = *cmd++) != '\0') 
    ;
  while (i <= len) 
    tmp[i++] = '\0';
}

bool isEmpty(char *str)
{
  return (*str == '\0');    
}


void handlePwd()
{
  char cwd[MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
  } else {
    printf("pwd: unable to print the working directory, returned to home\n$ ");
  }
}
