#include "hdr.h"

int main(int argc, char *argv[]) 
{
  setbuf(stdout, NULL);
  printf("$ ");

  char cmdque[MAX];
  int c;
  int i = 0;
  while ((c = getchar()) != EOF && i < MAX - 2) {
    if (c == '\n') {
      cmdque[i] = '\0';

      // Handling the exit command 
      if (strcmp(cmdque, exitCmd) == 0) break;

      if (strncmp(cmdque, typeCmd, 5) == 0) {
        handleType(cmdque + 5);
        flushCmds(cmdque, &i);
        continue;
      }

      // Handling the echo command 
      if (strncmp(cmdque, echoCmd, 5) == 0) {
        printf("%s\n$ ", cmdque + 5); // skips the first 5 things and prints them
        flushCmds(cmdque, &i);
        continue;
      }

      printf("%s: command not found\n$ ", cmdque);
      flushCmds(cmdque, &i);
      continue;
    }
    cmdque[i++] = c;
  }

  return 0;
}
