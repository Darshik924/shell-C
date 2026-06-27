#include "utils.h"

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

  char resolved[MAX];

  if (*path == '~') {
    char *home = getenv("HOME");
    if (home == NULL) {
      printf("cd: HOME not set\n");
      return;
    }

    if (*(path + 1) == '\0') {
      strncpy(resolved, home, MAX - 1);
      resolved[MAX - 1] = '\0';
    } else if (*(path + 1) == '/') {
      // '~/some/path'
      snprintf(resolved, MAX, "%s%s", home, path + 1);
    } else {
      // '~something'  is not a valid path
      printf("cd: invalid path: %s\n", path);
      return;
    }
    path = resolved;
  }

  if (*path != '/' && *path != '.' && *path != '~') {
    printf("cd: expected an absolute or a relative path to start with '/' or '.'\n");
    return;
  }

  if (chdir(path) == -1) {
    // Call chdir(): Pass the path string directly to chdir().
    // Handle Errors: If chdir() returns -1, the directory likely doesn't exist or you lack permissions. 
    printf("cd: %s: No such file or directory\n", path);
  }
}
