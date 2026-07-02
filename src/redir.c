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
    // Linux has specific descriptor Tables for each process(like 0 stdin,1 stdout, 2 stderr)
    // we will fool printf and change stdout descriptor table to that of file we want

    // Our program has a fixed #1 slot labelled as screen
    // int saved_fd = dup(1); // dup() will duplicate the "screen" number are write it to a new number and return that (say 5)

    int saved_fd = dup(isStdErr ? 2 : 1);
    // so before doing anything we make a copy of contact info on Page 1 or 2 (if its an err need to be redirected to a file) and later on we will restore the copy of terminal output
    // if we overwrite Page 1 and loose the "screen" no. then no way to get it back, this is just a save
    // Now you have two ways to reach the screen og #1 slot and the saved_fd slot
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
    // Extract the fileName

    // So here we want to redirect the output from Standard Output 1 (basically the screen) to a file instead
    // There is this phone Book analogy for linux descriptor tables. Imagine you have phone book and pages numbers for stuff
    // 0 is stdin, 1 is stdout, 2 is stderr
    // when we run printf, the program looks at 1 and see that text is to be printed to the screen

    int flags = O_WRONLY | O_CREAT | (isAppend ? O_APPEND : O_TRUNC);
    // Access modes are open write only, create if missing, append at the end file, truncate existing file to zero length
    int fd = open(fileName, flags, 0644); // this fd is the Page 6 (say)
    /* The OS opens out.txt and gives you a new temporary number (say 6) */
    // Nothing has been redirected yet; you just have the file ready to use.

    // O_CREAT - create if does not exist
    // O_TRUNC - if using > empty the file
    // O_APPEND - if using >> make sure text is appended
    if (fd < 0)
    {
        // handle errors to the file
        perror("Error opening file");
        close(saved_fd);
        return;
    }

    if (dup2(fd, isStdErr ? 2 : 1) < 0) // This is the specific command that changes the direction. dup2(source, target) means: "Copy the number from source and force it into target, destroying whatever was there before."
    {
        // handle erros to the file
        perror("dup2 failed");
        close(fd);
        close(saved_fd);
        return;
    }
    /* The system looks at slot #6 (fd), which holds the "File" number.
    It looks at slot #1 (the target). It sees "Screen" there.
    It erases "Screen" from slot #1.
    It writes "File" into slot #1  */
    close(fd);
    // cleaning up the temporary numbers
    
    printf("%s\n", st);
    // Your program (and printf) doesn't know anything changed. It is hard-coded to always look at slot #1. It looks at #1, sees a number, and dials it. 
    // It just happens to be the File's number now instead of the Screen's. The direction of data flow has physically changed because the definition of "Standard Output" (slot #1) has changed.

    if (dup2(saved_fd, isStdErr ? 2 : 1) < 0) // You are done writing to the file. If you end the program now, it’s fine. But if you want to keep running and print more things to the screen, you must fix slot #1
    {
        perror("dup2 restore");
        close(saved_fd);
        return;
    }
    close(saved_fd);

    // The core secret is that printf never changes. It always dials "1". You are simply changing the contact info stored at address "1" temporarily.
    printf("$ ");
    fflush(stdout);
}