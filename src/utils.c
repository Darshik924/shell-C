#include "hdr.h"
#include <limits.h>

// A function to flush our commands queue
void cleanCmds(char *cmd, int *x)
{
  while (*cmd != '\0')
    *cmd++ = '\0';
  *x = 0;
}

// A function to remove specifically trailing spaces
void trimDown(char *str)
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
    strncmp(cmd, exitCmd, 4) == 0 ||
    strcmp(cmd, pwdCmd) == 0 ||
    strcmp(cmd, cdCmd) == 0
  );
}

// Function that returns a pointer to the first occurence of a character c and returns NULL otherwise
char *strChr(char *name, int c) 
{
  while (*name != c) {
    ++name;
    if (*name == c) 
      return name;

    if (*name == '\0') 
      return NULL; 
  } 
  return NULL;
} 

void freeArgv(char **argv) 
{
  for (int i=0; argv[i]; ++i) 
    free(argv[i]);

  free(argv);
}

// Function to return the length of the string 
int strLen(char *str)
{
  int i = 0;
  if (str == NULL || *str == '\0') return 0;

  while (*str++ != '\0')
    ++i;
  return i;
}

// A function that trims the starting whitespaces in place for the string
void trimUp(char tmp[])
{ 
  char *cmd = tmp;
  trimDown(cmd);
  if (cmd == NULL || *cmd == '\0') return;
  int len = strLen(tmp);

  while (*cmd == ' ')
    ++cmd;
  int i = 0;
  while ((tmp[i++] = *cmd++) != '\0' && i < MAX - 1) 
    ;
  while (i <= len) 
    tmp[i++] = '\0';
}

bool isEmpty(char *str)
{
  return (*str == '\0');    
}

void formatCmd(char *cmd)
{
  bool sp_found = 0, isquote = 0, isDquote = 0;

  char *pt = cmd, *st = cmd;
  int len = strLen(cmd);

  while (*cmd != '\0') {

    if (*cmd == '\\' && isquote) {
      *pt++ = *cmd++;
      continue;
    }

    if (*cmd == '\\') {
      *pt++ = *++cmd;
      ++cmd;
      continue;
    }

    if (*cmd == '\"' && isquote) {
      *pt++ = *cmd++;
      continue;
    }

    if (*cmd == '\'' && isDquote){
      *pt++ = *cmd++;
      continue;
    }

    if (*cmd == '\"') {
      if (!isDquote) {
        isDquote = 1;
        sp_found = 0;
      } else isDquote = 0;
      ++cmd;
      continue;
    }

    if (*cmd == '\'') {
      if (!isquote) {
        isquote = 1;
        sp_found = 0;
      } else isquote = 0;
      ++cmd;
      continue;
    }

    if (isquote || isDquote) {
      *pt++ = *cmd++;
    } else {
      if (*cmd == ' ') {
        if (!sp_found) {
          *pt++ = *cmd++;
          ++sp_found;
        } else {
          ++cmd;
        }
      } else {
        *pt++ = *cmd++;
        sp_found = 0;
      }
    }
  }

  *pt = '\0';
  int l2 = strLen(st);
  
  while (l2++ < len) 
    *pt++ = '\0';
}
