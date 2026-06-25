#include "utils.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

char *findExecutable(char *name)
{
  if (strChr(name, '/'))
  {

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

  while (dir)
  {
    size_t need = strlen(dir) + 1 + strlen(name) + 1;

    char *p = malloc(need);

    snprintf(p, need, "%s/%s", dir, name);
    if (access(p, X_OK) == 0)
    {
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
  if (!s)
  {
    *outArgv = NULL;
    return 0;
  }

  int cap = 8, n = 0;
  char **argv = malloc(cap * sizeof(char *));
  if (!argv)
  {
    free(s);
    *outArgv = NULL;
    return 0;
  }

  int len = strlen(s);
  char *token = malloc(len + 1);
  if (!token)
  {
    free(s);
    free(argv);
    *outArgv = NULL;
    return 0;
  }

  bool inSingleQuote = false, inDoubleQuote = false;
  int toklen = 0;

  for (int i = 0; i <= len; ++i)
  {
    char c = s[i];

    if (c == '\\' && inSingleQuote)
    {
      token[toklen++] = c;
      continue;
    }

    if (c == '\\')
    {
      if (i < len)
        ++i;
      c = s[i];
      token[toklen++] = c;
      continue;
    }

    // Handle double quotes
    if (c == '\"')
    {
      if (inSingleQuote)
      {
        // Double quote inside single Quotes is literal
        token[toklen++] = c;
      }
      else
      {
        inDoubleQuote = !inDoubleQuote;
        // Toggle State
      }
      continue;
    }

    // Handle single quotes
    if (c == '\'')
    {
      if (inDoubleQuote)
      {
        // Single quote inside double quotes is literal
        token[toklen++] = c;
      }
      else
      {
        // Toggle single quote mode
        inSingleQuote = !inSingleQuote;
      }
      continue;
    }

    // Space is delimiter only outside both quote types
    if (c == ' ' && (!inSingleQuote && !inDoubleQuote))
    {
      if (toklen > 0)
      {
        token[toklen] = '\0';
        if (n + 1 >= cap)
        {
          cap *= 2;
          argv = realloc(argv, cap * sizeof(char *));
        }
        argv[n++] = strdup(token);
        toklen = 0;
      }
      continue;
    }

    if (c == '\0')
    {
      if (toklen > 0)
      {
        token[toklen] = '\0';
        if (n + 1 >= cap)
        {
          cap *= 2;
          argv = realloc(argv, cap * sizeof(char *));
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
  int argc = buildArgv(cmdline, &argv);

  if (argc == 0)
  {
    freeArgv(argv);
    printf("$ ");
    return true;
  }

  // 1. Check for Redirection (support ">", "1>", "2>", ">file", "1>file", and "2>file")
  char *outputFile = NULL;
  int redirectIndex = -1;
  int redirectFd = 1;

  for (int i = 0; argv[i] != NULL; i++)
  {
    char *a = argv[i];

    if (strcmp(a, ">") == 0 || strcmp(a, "1>") == 0)
    {
      if (argv[i + 1] != NULL)
      {
        redirectFd = 1;
        outputFile = argv[i + 1];
        redirectIndex = i;
        argv[i] = NULL;
        break;
      }
      else
      {
        printf("Syntax error: missing filename after '%s'\n$ ", a);
        freeArgv(argv);
        return true;
      }
    }

    if (strcmp(a, "2>") == 0)
    {
      if (argv[i + 1] != NULL)
      {
        redirectFd = 2;
        outputFile = argv[i + 1];
        redirectIndex = i;
        argv[i] = NULL;
        break;
      }
      else
      {
        printf("Syntax error: missing filename after '%s'\n$ ", a);
        freeArgv(argv);
        return true;
      }
    }

    // Handle attached filename forms: ">file", "1>file" or "2>file"
    if (a[0] == '>' && a[1] != '\0')
    {
      redirectFd = 1;
      outputFile = a + 1;
      redirectIndex = i;
      argv[i] = NULL;
      break;
    }

    if (a[0] == '1' && a[1] == '>' && a[2] != '\0')
    {
      redirectFd = 1;
      outputFile = a + 2;
      redirectIndex = i;
      argv[i] = NULL;
      break;
    }

    if (a[0] == '2' && a[1] == '>' && a[2] != '\0')
    {
      redirectFd = 2;
      outputFile = a + 2;
      redirectIndex = i;
      argv[i] = NULL;
      break;
    }
  }

  char *full = findExecutable(argv[0]);

  if (!full)
  {
    printf("%s: not found\n$ ", argv[0]);
    freeArgv(argv);
    return true;
  }

  pid_t pid = fork();

  if (pid == 0)
  {
    // --- CHILD PROCESS ---

    // 2. Handle Redirection if detected
    if (outputFile != NULL)
    {
      int fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fd < 0)
      {
        perror("open");
        _exit(1);
      }

      // Redirect the selected fd to the file
      if (dup2(fd, redirectFd) < 0)
      {
        perror("dup2");
        _exit(1);
      }
      close(fd); // fd is no longer needed, redirectFd now points to the file
    }

    execv(full, argv);

    // If execv returns, it failed
    perror("execv");
    _exit(127);
  }
  else if (pid < 0)
  {
    // --- FORK FAILED ---
    perror("fork");
  }
  else
  {
    // --- PARENT PROCESS ---
    int status;
    waitpid(pid, &status, 0);
  }

  free(full);
  freeArgv(argv); // Frees the modified argv (where '>' was set to NULL)
  printf("$ ");
  return true;
}