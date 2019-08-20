#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "list.h"
#include "pwd.h"
#include "echo.h"
#include "cd.h"

#define clear() printf("\033[H\033[J")

void chooseCommand(char *str)
{
}