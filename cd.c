#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

void cd(char *str)
{
    int x = chdir(str);
    if (x == -1)
        perror("Error ");
}