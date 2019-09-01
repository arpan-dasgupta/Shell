#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

void setenviron(char *var, char *val)
{
    setenv(var, val, 1);
}
void unsetenviron(char *var)
{
    unsetenv(var);
}