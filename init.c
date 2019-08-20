#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")

void initialise(char *username)
{
    clear();
    // char *username = getenv("USER");
    printf("Welcome to ASH ( Arpan's SHell )\n");
    printf("User - %s\n", username);
    // sleep(3);
}