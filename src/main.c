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
        cleanCmds(cmdque, &i);
        continue;
      }
      // Our cmd at this point is not empty and trimmed down
      trimUp(cmdque);
      // Cmd que at this pt is trimmed totally Next look for the builtin commands
      formatCmd(cmdque);
      // formatCmd function will remove all the extra spaces except the quotes and with quotes it will give described behaviour to our string all editing the string in place
      /* Make a formatCmd function that formats our cmd with using single and double quotes at this point */

      // Handling the pwd command
      if (strcmp(cmdque, pwdCmd) == 0) {
        handlePwd();
        printf("$ ");
        cleanCmds(cmdque, &i);
        continue;
      }

      // Handling the cd command 
      if (strncmp(cmdque, cdCmd, 2) == 0) {
        handleCd(cmdque + 2);
        printf("$ ");
        cleanCmds(cmdque, &i);
        continue;
      }

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
        printf("%s\n$ ", cmdque + 5);
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
