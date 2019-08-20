#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>
#include "init.h"
#include "list.h"
#include "pwd.h"
#include "echo.h"
#include "cd.h"

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
    // pwd();
    // cd("/home/arpan/Documents");
    // pwd();
    // cd("~");
    // pwd();
    // listfiles("~");
    while (1)
    {
        char curdir[1024];
        char input[100];
        getcwd(curdir, sizeof(curdir));
        printf("%s @ %s >>> ", username, curdir);
        scanf(" %s", input);
        printf("%s ", input);
        break;
    }

    return 0;
}
