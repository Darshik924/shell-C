#include "hdr.h"

void handlePwd()
{
  char cwd[MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) { 
    // getcwd() a function to simple get the current working directory
    printf("%s\n", cwd);
  } else {
    printf("pwd: unable to print the working directory, returned to home\n$ ");
  }
}

void handleCd(char *path)
{
  while (*path == ' ') 
    ++path;

  if (path == NULL || strLen(path) == 0 || *path == '\0') {
    printf("cd: error no path supplied\n");
    return;
  }

  if (*path != '/' && *path != '.') {
    printf("cd: expected an absolute or a relative path to start with '/' or '.'\n");
    return;
  }

  if (chdir(path) == -1) {
    // Call chdir(): Pass the path string directly to chdir().
    // Handle Errors: If chdir() returns -1, the directory likely doesn't exist or you lack permissions. 
    printf("cd: %s: No such file or directory\n", path);
  }
}
