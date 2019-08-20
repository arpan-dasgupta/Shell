#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define makered printf("\033[1;31m")
#define makedef printf("\033[0m")
#define clear() printf("\033[H\033[J")

void initialise(char *username)
{
    clear();
    // char *username = getenv("USER");
    makered;
    printf("Welcome to ASH ( Arpan's SHell )\n");
    printf("User - %s\n\n", username);
    makedef;
    // sleep(3);
}