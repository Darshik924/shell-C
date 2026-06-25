#ifndef DECL
#define DECL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>

#define MAX 1024
/*
   F_OK: Tests only for the existence of the file.
   R_OK: Tests for read permission.
   W_OK: Tests for write permission.
   X_OK: Tests for execute permission (or search permission for directories).
*/

extern char exitCmd[];
extern char echoCmd[];
extern char typeCmd[];
extern char pwdCmd[];
extern char cdCmd[];

void cleanCmds(char *, int *);
void handleType(char *, char *[]);
void trimDown(char *);

bool isBuiltin(char *);
char *findExecutable(char *);
char *strChr(char *, int);

char *getcwd(char *, size_t);
void freeArgv(char **);
bool runExternal(char *);

bool isEmpty(char *);
void trimUp(char []);
int strLen(char *);

void handlePwd();
void handleCd(char *);
void formatCmd(char *);

#endif