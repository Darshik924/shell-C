#include "utils.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void handleEcho(char *cmd)
{
    char *st = cmd;
    char fileName[MAX];
    bool isDirected = false;
    bool isDirected1 = false; // For '1>' (explicit stdout)
    char *redirectPos = NULL;
    char *scan = cmd; // Use a separate pointer for scanning to preserve 'st'

    // 1. Scan for '>' or '1>'
    while (*scan != '\0')
    {
        if (*scan == '>' && (scan == cmd || *(scan - 1) != '1'))
        {
            // Check if it's just '>' (and not part of '1>')
            // If input is " >", this one catches it.
            redirectPos = scan;
            isDirected = true;
            break;
        }
        else if (*scan == '1' && *(scan + 1) == '>')
        {
            redirectPos = scan;
            isDirected1 = true;
            break;
        } else if (*scan == '2' && *(scan + 1) == '>') {
            isDirected1 = true;
            isDirected= true;
            // *scan = '\0';
            // Alice is a cannot be cruel 2> 
            while (*--scan == ' ')
                ;
            *++scan = '\0';
            break;
        }
        scan++;
    }

    if (!isDirected && !isDirected1)
    {
        printf("%s\n$ ", st);
    }
    else
    {
        // Save the original Terminal Output (fd 1)
        int saved_stdout = dup(1);
        if (saved_stdout < 0)
        {
            perror("dup");
            return;
        }

        // Parse the filename
        *redirectPos = '\0'; // Terminate message string

        char *fnPtr = isDirected1 ? redirectPos + 2 : redirectPos + 1;
        while (*fnPtr == ' ' || *fnPtr == '\t')
            fnPtr++;

        int i = 0;
        while (fnPtr[i] != '\0' && fnPtr[i] != ' ' && fnPtr[i] != '\n')
            fileName[i++] = fnPtr[i];
        fileName[i] = '\0';

        // Open File
        int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("Error opening file");
            close(saved_stdout);
            return;
        }

        // Redirect stdout to file
        if (dup2(fd, 1) < 0)
        {
            perror("dup2 failed");
            close(fd);
            close(saved_stdout);
            return;
        }
        close(fd);

        // Write ONLY the message to the file
        printf("%s\n", st);

        // RESTORE stdout to terminal
        if (dup2(saved_stdout, 1) < 0)
        {
            perror("dup2 restore");
            close(saved_stdout);
            return;
        }
        close(saved_stdout);

        printf("$ ");
        fflush(stdout); // Ensure prompt appears immediately
    }
}