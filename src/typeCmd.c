#include "hdr.h"

// type command Handler
void handleType(char *cmd, char *argv[]) 
{
  if (cmd == NULL) {
    printf("$ ");
    return;
  }
  
  // Extract the target first
  char target[MAX];
  size_t i = 0;
  while (*cmd && *cmd != ' ' && *cmd != '\0' && i < MAX-1 ) 
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
  /* 
    Searches the environment list for a string matching the provided name and returns a pointer to the corresponding value if found
  */
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

    strtok_r is the reentrant (thread-safe) variant of the standard C string tokenizer strtok
    saveptr: A pointer to a char * variable used to maintain parsing context between calls, enabling thread safety and nested tokenization.
    
    strtok_r uses the user-provided saveptr pointer.
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