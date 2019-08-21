#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

int otherCommands(char *com, char **str, int bg)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("\nFailed forking child..");
        return -1;
    }
    else if (pid == 0)
    {
        if (execvp(str[0], str) < 0)
            printf("Could not execute command..\n");
        return -1;
    }
    else
    {
        int status;
        if (bg == 0)
            waitpid(pid, &status, 0);
    }
    return pid;
}