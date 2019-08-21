#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>
#include "command.h"
#include "init.h"
#include "list.h"
#include "pwd.h"
#include "echo.h"
#include "cd.h"

#define makeblue printf("\033[1;34m")
#define makedef printf("\033[0m")
#define clear() printf("\033[H\033[J")

int working_proc[2048], status[2048], count = 0, fin = 0;
int main(int argc, char const *argv[])
{
    char *username = getenv("USER");
    initialise(username);
    char home[1024];
    getcwd(home, sizeof(home));
    // printf("%s > ", username);
    // printf("\n");

    // echo("hi");
    // pwd();
    // cd(home, "/home/arpan/Documents/..");
    // pwd();
    // cd(home, "~/Test");
    // pwd();
    // listfiles(home, "~/../Shell", "");
    // listfiles(home, "/home/arpan/Documents/..", "");
    while (1)
    {
        char curdir[1024];
        char input[1024], ch;
        char *sys_name = getenv("SESSION");
        getcwd(curdir, sizeof(curdir));
        int i, f = 1;

        for (i = 0; i < count; i++)
        {
            if (status[i] == 0)
                continue;
            // printf("%d\n", working_proc[i]);
            int sst;
            int v = waitpid(working_proc[i], &sst, WNOHANG);
            if (v == -1)
            {
                perror("waitpid failed");
            }
            if (v == 0)
            {
                continue;
            }
            if (WIFEXITED(sst))
            {
                const int es = WEXITSTATUS(sst);
                printf("Process with PID - %d\n", working_proc[i]);
                if (es == 0)
                {
                    printf("Exited normally\n");
                }
                else
                {
                    printf("Exited with exit code %d", es);
                }
                status[i] = 0;
                fin++;
            }
        }
        if (count - fin > 0)
            printf("%d background processes\n", count - fin);

        for (i = 0; i < strlen(home); i++)
        {
            if (curdir[i] != home[i])
                f = 0;
        }
        if (f == 1)
        {
            curdir[0] = '~';
            int f = 1;
            for (; i < strlen(curdir); i++)
                curdir[f++] = curdir[i];
            curdir[f] = '\0';
        }
        makeblue;
        printf("<%s@%s:%s> >> ", username, sys_name, curdir);
        makedef;
        int cur = 0;
        while ((ch = getchar()) != '\n')
        {
            input[cur++] = ch;
        }
        input[cur] = '\0';
        int *a = chooseCommand(home, input);
        for (i = 1; i <= a[0]; i++)
        {
            working_proc[count++] = a[i];
            status[count - 1] = 1;
        }
    }

    return 0;
}
