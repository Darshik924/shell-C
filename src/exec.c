#include "hdr.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char *findExecutable(char *name) 
{
  if (strChr(name, '/')) {

    if (access(name, X_OK) == 0) 
        return strdup(name);

    return NULL;
  }
  char *path = getenv("PATH");

  if (!path) 
    return NULL;

  char *dup = strdup(path);
  char *saveptr = NULL;
  char *dir = strtok_r(dup, ":", &saveptr);

  while (dir) {
    size_t need = strlen(dir) + 1 + strlen(name) + 1;

    char *p = malloc(need);

    snprintf(p, need, "%s/%s", dir, name);
    if (access(p, X_OK) == 0) { 
        free(dup); 
        // If he finds an executable using that he returns p
        return p; 
    }
    free(p);
    dir = strtok_r(NULL, ":", &saveptr);
  }
  free(dup);
  return NULL;
}

int buildArgv(char *cmdline, char ***outArgv) 
{
  char *s = strdup(cmdline);
  if (!s) {
    *outArgv = NULL;
    return 0;
  }

  int cap = 8, n = 0;
  char **argv = malloc(cap * sizeof(char*));
  if (!argv) {
    free(s);
    *outArgv = NULL;
    return 0;
  }

  int len = strlen(s);
  char *token = malloc(len + 1);
  if (!token) {
    free(s);
    free(argv);
    *outArgv = NULL;
    return 0;
  }

  bool inSingleQuote = false, inDoubleQuote = false;
  int toklen = 0;

  for (int i = 0; i <= len; ++i) {
    char c = s[i];

    if (c == '\\') {
      if (i < len) ++i;
      c = s[i];
      token[toklen++] = c;
    }

    // Handle double quotes
    if (c == '\"') {
      inDoubleQuote = !inDoubleQuote;
      continue;
    }

    // Handle single quotes
    if (c == '\'') {
      if (inDoubleQuote) {
        // Single quote inside double quotes is literal
        token[toklen++] = c;
      } else {
        // Toggle single quote mode
        inSingleQuote = !inSingleQuote;
      }
      continue;
    }

    // Space is delimiter only outside both quote types
    if (c == ' ' && (!inSingleQuote && !inDoubleQuote)) {
      if (toklen > 0) {
        token[toklen] = '\0';
        if (n+1 >= cap) {
          cap *= 2;
          argv = realloc(argv, cap * sizeof(char*));
        }
        argv[n++] = strdup(token);
        toklen = 0;
      }
      continue;
    }

    if (c == '\0') {
      if (toklen > 0) {
        token[toklen] = '\0';
        if (n+1 >= cap) {
          cap *= 2;
          argv = realloc(argv, cap * sizeof(char*));
        }
        argv[n++] = strdup(token);
      }
      break;
    }

    token[toklen++] = c;
  }

  argv[n] = NULL;
  free(token);
  free(s);

  *outArgv = argv;
  return n;
}

// Our main entry point of executing a shell command
// What this function does is recieves cmdline, calls buildArgv using cmdline and &argv which builds our command
// If executable is found just fork and return it. Else prints an error 

bool runExternal(char *cmdline) 
{
  char **argv;
  // ls -l /tmp NULL
  int argc = buildArgv(cmdline, &argv);
  // Above line converts this string into an array like as execv() function requires this format 

  if (argc == 0) { 
    freeArgv(argv); 
    printf("$ "); 
    return true; 
  }   

  char *full = findExecutable(argv[0]);
  // This above finds the actual path for the command name

  if (!full) { 
    printf("%s: not found\n$ ", argv[0]); 
    freeArgv(argv); 
    return true;
  }

  pid_t pid = fork();

  if (pid == 0) {
    execv(full, argv);
    // Requires a NULL terminated array so we fill a NULL at the end

    perror("execv");
    _exit(127);
  } else if (pid < 0) {
    perror("fork");
  } else {

    int status;
    waitpid(pid, &status, 0);
  }
  free(full);
  freeArgv(argv);
  printf("$ ");
  return true;
}