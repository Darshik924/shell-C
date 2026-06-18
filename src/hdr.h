#ifndef DECL
#define DECL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

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

void cleanCmds(char *, int *);
void handleType(char *, char *[]);
void triM(char *);

#endif