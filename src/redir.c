#include "utils.h"

void handleEcho(char *cmd)
{
    char *st = cmd;
    char mes[MAX];
    char fileName[MAX];
    bool isDirected = false;
    char *redirectPos = NULL;
    while (*cmd != '\0')
    {
        if (*cmd == '>')
        {
            redirectPos = cmd;
            isDirected = true;
            break;
        }
        cmd++;
    }

    if (!isDirected)
        printf("%s\n", st);
    else
    {
        // 2. Parse the filename
        // Terminate the message string at '>'
        *redirectPos = '\0';

        // Move pointer past '>'
        char *fnPtr = redirectPos + 1;

        // Skip leading whitespace in filename
        while (*fnPtr == ' ' || *fnPtr == '\t')
            fnPtr++;

        // Extract filename (stop at next space or end)
        int i = 0;
        while (fnPtr[i] != '\0' && fnPtr[i] != ' ' && fnPtr[i] != '\n')
        {
            fileName[i] = fnPtr[i];
            i++;
        }
        fileName[i] = '\0';

        // 3. Open the file
        // O_WRONLY: Write only
        // O_CREAT: Create if doesn't exist
        // O_TRUNC: Overwrite if exists (crucial for '>')
        // 0644: Permissions (rw-r--r--)
        int fd = open(fileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0)
        {
            perror("Error opening file");
            return;
        }

        // 4. Redirect Standard Output (fd 1) to the file
        // dup2(fd, 1) makes fd 1 point to the same file as fd
        if (dup2(fd, 1) < 0)
        {
            perror("dup2 failed");
            close(fd);
            return;
        }

        // Close the original fd, as fd 1 is now the duplicate
        close(fd);

        // 5. Perform the output
        // Since we redirected fd 1, printf now writes to the file
        printf("%s\n", st);

        // Optional: Restore stdout if you need to print more to terminal later
        // For a simple command execution, the process often exits or execs here.
        // If this is a built-in and you stay in the shell, you might want to save
        // the original stdout (int saved_stdout = dup(1);) before dup2 and restore it after.
    }
}