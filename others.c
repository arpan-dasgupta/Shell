#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

void otherCommands(char *com, char **str, int bg)
{
    printf("%d ++ ", bg);
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("\nFailed forking child..");
        return;
    }
    else if (pid == 0)
    {
        if (execvp(str[0], str) < 0)
            printf("Could not execute command..\n");
        return;
    }
    else
    {
        int status;
        if (bg == 0)
            waitpid(pid, &status, 0);
    }
    return;
}