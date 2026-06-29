#include "utils.h"

void handleEcho(char *cmd)
{
    char *st = cmd;
    char fileName[MAX];
    bool hasRedirect = false;
    bool isStdErr = false;
    bool isAppend = false;
    /* Gettign States for our echo command on which depends what decision to take */
    char *redirectPos = NULL;
    int operatorLen = 0;
    char *scan = cmd;

    while (*scan != '\0')
    {
        if (*scan == '>' && *(scan + 1) == '>')
        {
            hasRedirect = true;
            redirectPos = scan;
            operatorLen = 2;
            isAppend = true;
            break;
        }
        if (*scan == '1' && *(scan + 1) == '>' && *(scan + 2) == '>')
        {
            hasRedirect = true;
            redirectPos = scan;
            operatorLen = 3;
            isAppend = true;
            break;
        }
        if (*scan == '2' && *(scan + 1) == '>' && *(scan + 2) == '>')
        {
            hasRedirect = true;
            redirectPos = scan;
            operatorLen = 3;
            isAppend = true;
            isStdErr = true;
            break;
        }
        if (*scan == '>' && (scan == cmd || *(scan - 1) != '1'))
        {
            hasRedirect = true;
            redirectPos = scan;
            operatorLen = 1;
            break;
        }
        if (*scan == '1' && *(scan + 1) == '>')
        {
            hasRedirect = true;
            redirectPos = scan;
            operatorLen = 2;
            break;
        }
        if (*scan == '2' && *(scan + 1) == '>')
        {
            hasRedirect = true;
            redirectPos = scan;
            operatorLen = 2;
            isStdErr = true;
            break;
        }
        scan++;
    }

    if (!hasRedirect)
    {
        printf("%s\n$ ", st);
        return;
    }

    int saved_fd = dup(isStdErr ? 2 : 1);
    if (saved_fd < 0)
    {
        perror("dup");
        return;
    }

    *redirectPos = '\0';

    char *fnPtr = redirectPos + operatorLen;
    while (*fnPtr == ' ' || *fnPtr == '\t')
        fnPtr++;

    int i = 0;
    while (fnPtr[i] != '\0' && fnPtr[i] != ' ' && fnPtr[i] != '\n' && fnPtr[i] != '\t')
        fileName[i++] = fnPtr[i];
    fileName[i] = '\0';

    int flags = O_WRONLY | O_CREAT | (isAppend ? O_APPEND : O_TRUNC);
    // Access modes are open write only, create if missing, append at the end file, truncate existing file to zero length
    int fd = open(fileName, flags, 0644);
    /* Open the file for reading or writing to the file */
    if (fd < 0)
    {
        // handle errors to the file
        perror("Error opening file");
        close(saved_fd);
        return;
    }

    if (dup2(fd, isStdErr ? 2 : 1) < 0)
    {
        // handle erros to the file
        perror("dup2 failed");
        close(fd);
        close(saved_fd);
        return;
    }
    close(fd);

    printf("%s\n", st);

    if (dup2(saved_fd, isStdErr ? 2 : 1) < 0)
    {
        perror("dup2 restore");
        close(saved_fd);
        return;
    }
    close(saved_fd);

    printf("$ ");
    fflush(stdout);
}