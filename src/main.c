#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  printf("$ ");

  char commands[MAX];
  int c;
  int i = 0;
  while ((c = getchar()) != EOF && i < MAX - 2) {
    if (c == '\n') {
      commands[i] = '\0';
      printf("%s: command not found\n$ ", commands);
      while (i) 
        commands[i--] = '\0';
      commands[i] = '\0';
      continue;
    }

    commands[i++] = c;
  }
  printf("\n");

  return 0;
}
