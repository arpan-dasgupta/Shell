#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include "globalVar.h"

#define clear() printf("\033[H\033[J")

int otherCommands(char *com, char **str, int bg) {
  pid_t pid = fork();
  if (pid == -1) {
    printf("\nFailed forking child..");
    return -1;
  } else if (pid == 0) {
    if (bg == 1) {
      setpgid(0, 0);
      close(0);
      close(1);
      close(2);
    }
    int rv = execvp(str[0], str);
    printf("%d ", rv);
    if (rv < 0) printf("ERROR: Invalid command\n");
    return -1;
  } else {
    int status;
    if (bg == 0) {
      curPID = pid;
      waitpid(pid, &status, 0);
    }
  }
  return pid;
}