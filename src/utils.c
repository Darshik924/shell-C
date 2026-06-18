#include "hdr.h"

void flushCmds(char *cmd, int *x)
{
  while ((*x)-- > 0)
    *cmd++ = '\0';
  *x = 0;
}

void handleType(char *cmd) 
{
  if (strncmp(cmd, echoCmd, 4) == 0) {
    printf("%sis a shell builtin\n$ ", echoCmd);
  } else if (strncmp(cmd, typeCmd, 4) == 0) {
    printf("%sis a shell builtin\n$ ", typeCmd);
  } else if (strncmp(cmd, exitCmd, 4) == 0) {
    printf("%s is a shell builtin\n$ ", exitCmd);
  } else {
    printf("%s: not found\n$ ", cmd);
  }
}