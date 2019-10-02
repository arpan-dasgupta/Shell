#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#define clear() printf("\033[H\033[J")
int rv;
void setenviron(char *var, char *val) {
  rv = setenv(var, val, 1);
  if (rv < 0) perror("Error: ");
}
void unsetenviron(char *var) {
  rv = unsetenv(var);
  if (rv < 0) perror("Error: ");
}