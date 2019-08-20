#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

void echo(char *str)
{
    printf("%s", str);
}