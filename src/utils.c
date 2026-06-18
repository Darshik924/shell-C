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
void triM(char *str)
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

// type command Handler
void handleType(char *cmd, char *argv[]) 
{
  if (cmd == NULL) {
    printf("$ ");
    return;
  }
  while (*cmd == ' ') 
    ++cmd;
  
  // Extract the target first
  char target[MAX];
  size_t i = 0;
  while (*cmd && *cmd != ' ' && i < MAX-1) 
    target[i++] = *cmd++;
  target[i] = '\0';

  if (i == 0) {
    printf("$ ");
    return;
  }

  if (isBuiltin(target)) {
    printf("%s is a shell builtin\n$ ", target);
    return;
  }

  char *path = getenv("PATH");
  if (path == NULL) {
    printf("%s: not found\n$ ", target);
    return;
  }

  char *pathdup = strdup(path);
  /* 
    The strdup() function in C duplicates a given null-terminated string by dynamically allocating memory and copying the content, returning a pointer to the new string
  */

  if (pathdup == NULL) {
    printf("%s: not found\n$ ", target);
    return;
  }

  char *saveptr = NULL;
  char *dir = strtok_r(pathdup, ":", &saveptr);
  /*  
    The strtok() function is that splits a string into smaller parts based on a set of delimiters.  
    It works by scanning the input string, replacing each encountered delimiter with a null terminator (\0), and returning a pointer to the beginning of the extracted sequence 
  */

  while (dir != NULL) {
    size_t need = strlen(dir) + 1 + strlen(target) + 1;

    char *fpath = malloc(need);
    if (fpath == NULL) break;

    snprintf(fpath, need, "%s/%s", dir, target);

    if (access(fpath, X_OK) == 0) {
      printf("%s is %s\n$ ", target, fpath);
      free(fpath);
      free(pathdup);
      return;
    }

    free(fpath);
    dir = strtok_r(NULL, ":", &saveptr);
  }

  free(pathdup);
  printf("%s: not found\n$ ", target);
}