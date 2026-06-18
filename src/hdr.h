#ifndef DECL
#define DECL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

extern char exitCmd[];
extern char echoCmd[];
extern char typeCmd[];
extern char cmdque[];

void flushCmds(char *, int *);
void handleType(char *);

#endif // HDR_H