#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "globalVar.h"

#define clear() printf("\033[H\033[J")

int otherCommands(char *com, char **str, int bg) {
  pid_t pid = fork();
  if (pid == -1) {
    printf("\nFailed forking child..");
    return -1;
  } else if (pid == 0) {
    setpgid(0, 0);
    int rv = execvp(str[0], str);
    // printf("%d ", rv);
    if (rv < 0) printf("ERROR: Invalid command\n");
    return -1;
  } else {
    curPID = pid;
    if (bg == 0) {
      signal(SIGTTIN, SIG_IGN);
      signal(SIGTTOU, SIG_IGN);
      setpgid(pid, 0);
      tcsetpgrp(0, __getpgid(pid));
      int st;
      waitpid(pid, &st, WUNTRACED);
      tcsetpgrp(0, getpgrp());
      signal(SIGTTIN, SIG_DFL);
      signal(SIGTTOU, SIG_DFL);
      if (WIFSTOPPED(st)) {
        // printf("ok\n");
        working_proc[Proccount].pid = pid;
        strcpy(working_proc[Proccount].pname, com);
        status[Proccount] = 1;
        Proccount++;
      }
    }
  }
  return pid;
}