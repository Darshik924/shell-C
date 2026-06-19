#include "hdr.h"

int main(int argc, char *argv[]) 
{
  setbuf(stdout, NULL);
  printf("$ ");

  char cmdque[MAX];
  int c, i = 0;
  while ((c = getchar()) != EOF && i < MAX - 2) {
    if (c == '\n') {
      cmdque[i] = '\0';
      trimDown(cmdque);
      
      if (isEmpty(cmdque)) {
        printf("$ ");
        cleanCmds(cmdque);
        continue;
      }
      // Our cmd at this point is not empty and trimmed down
      // Next Implement trimUp but trim cmdque in place 
      // May Implement using a index at point if != ' ' || != '\0' and then copy all their contents to the start, May also need to write a custom length function

      // Handling the exit command 
      if (strcmp(cmdque, exitCmd) == 0) break;

      // Handling the type command
      if (strncmp(cmdque, typeCmd, 5) == 0) {
        handleType(cmdque + 5, argv);
        cleanCmds(cmdque, &i);
        continue;
      }

      // Handling the echo command 
      if (strncmp(cmdque, echoCmd, 5) == 0) {
        printf("%s\n$ ", cmdque + 5); // skips the first 5 things and prints them
        cleanCmds(cmdque, &i);
        continue;
      }

      if (runExternal(cmdque)){
        cleanCmds(cmdque, &i);
        continue;
      }
      // Function handles the cases of searching for the file and then building up the command and executing it along with error cases
      /* ALL the builtin commands come till here  */

      printf("%s: command not found\n$ ", cmdque);
      cleanCmds(cmdque, &i);
      continue;
    }
    cmdque[i++] = c;
  }

  return 0;
}
