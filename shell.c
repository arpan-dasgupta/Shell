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

int main(int argc, char const *argv[])
{
    char *username = getenv("USER");
    initialise(username);
    char home[1024];
    getcwd(home, sizeof(home));
    // printf("%s > ", username);
    // printf("\n");

    // echo("hi");
    pwd();
    cd(home, "/home/arpan/Documents/..");
    pwd();
    cd(home, "~/Test");
    pwd();
    // listfiles(home, "~/../Shell", "");
    // listfiles(home, "/home/arpan/Documents/..", "");
    // while (1)
    // {
    //     char curdir[1024];
    //     char input[1024];
    //     char *sys_name = getenv("SESSION");
    //     getcwd(curdir, sizeof(curdir));
    //     int i, f = 1;
    //     for (i = 0; i < strlen(home); i++)
    //     {
    //         if (curdir[i] != home[i])
    //             f = 0;
    //     }
    //     if (f == 1)
    //     {
    //         curdir[0] = '~';
    //         int f = 1;
    //         for (; i < strlen(curdir); i++)
    //             curdir[f++] = curdir[i];
    //         curdir[f] = '\0';
    //     }
    //     makeblue;
    //     printf("< %s @ %s:%s > >> ", username, sys_name, curdir);
    //     makedef;
    //     scanf(" %s", input);
    //     chooseCommand(input);
    // }

    return 0;
}
