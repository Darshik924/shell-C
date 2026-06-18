#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

void flushCmds(char *cmd, int *x)
{
  while ((*x)-- > 0)
    *cmd++ = '\0';
  *x = 0;
}

int main(int argc, char *argv[]) 
{
  setbuf(stdout, NULL);
  printf("$ ");

  char commands[MAX];
  char exit[] = "exit";
  int c;
  int i = 0;
  while ((c = getchar()) != EOF && i < MAX - 2) {
    if (c == '\n') {
      commands[i] = '\0';

      // Handling the exit commands
      if (strcmp(commands, exit) == 0) {
        break;
      }

      // Handling the echo commands
      if (strncmp(commands, "echo ", 5) == 0) {
        printf("%s\n$ ", commands + 5); // skips the first 5 things and prints them
        flushCmds(commands, &i);
        continue;
      }

      printf("%s: command not found\n$ ", commands);
      flushCmds(commands, &i);
      continue;
    }
    commands[i++] = c;
  }
  // printf("\n");

  return 0;
}
