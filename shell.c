#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>
#include "command.h"
#include "init.h"
#include "list.h"
#include "pwd.h"
#include "echo.h"
#include "cd.h"
#include "globalVar.h"

#define makeblue printf("\033[1;34m")
#define makedef printf("\033[0m")
#define clear() printf("\033[H\033[J")

int fin = 0;
int main(int argc, char const *argv[]) {
  Proccount = 0;
  char *username = getenv("USER");
  initialise(username);
  char home[1024];
  getcwd(home, sizeof(home));
  char sys_name[102];
  int ss = 0;
  {
    FILE *status = fopen("/proc/sys/kernel/hostname", "r");
    while (1) {
      char c = fgetc(status);
      if (feof(status)) {
        break;
      }
      sys_name[ss++] = c;
    }
    sys_name[ss - 1] = '\0';
  }
  while (1) {
    char curdir[1024];
    getcwd(curdir, sizeof(curdir));
    char input[1024], ch;
    int i, f = 1;

    for (i = 0; i < Proccount; i++) {
      if (status[i] == 0) continue;
      int sst;
      int v = waitpid(working_proc[i].pid, &sst, WNOHANG);
      if (v == 0) {
        continue;
      }
      //   if (v == -1) {
      //     perror("waitpid failed");
      //     continue;
      //   }
      if (v == -1 || WIFEXITED(sst)) {
        const int es = WEXITSTATUS(sst);
        printf("Process %s with PID - %d\n", working_proc[i].pname,
               working_proc[i].pid);
        if (es == 0 && v != -1) {
          printf("Exited normally\n");
        } else if (v != -1) {
          printf("Exited with exit code %d\n", es);
        } else {
          printf("Killed \n");
        }
        status[i] = 0;
        fin++;
      }
    }
    // if (Proccount - fin > 0)
    //     printf("%d background processes\n", Proccount - fin);

    for (i = 0; i < strlen(home); i++) {
      if (curdir[i] != home[i]) f = 0;
    }
    if (f == 1 &&
        (curdir[strlen(home)] == '/' || curdir[strlen(home)] == '\0')) {
      curdir[0] = '~';
      int f = 1;
      for (; i < strlen(curdir); i++) curdir[f++] = curdir[i];
      curdir[f] = '\0';
    }
    makeblue;
    printf("<%s@%s:%s> >> ", username, sys_name, curdir);
    makedef;
    int cur = 0;
    while ((ch = getchar()) != '\n') {
      input[cur++] = ch;
    }
    input[cur] = '\0';
    struct comm *a = runCommand(home, input);
    for (i = 1; i <= a[0].pid; i++) {
      working_proc[Proccount].pid = a[i].pid;
      strcpy(working_proc[Proccount].pname, a[i].pname);
      printf("%s %d\n", working_proc[Proccount].pname,
             working_proc[Proccount].pid);
      Proccount++;
      status[Proccount - 1] = 1;
    }
    if (a[0].jobs) {
      for (int i = 0; i < Proccount; i++) {
        if (status[i] == 1)
          printf("[%d] Running ", i + 1);
        else
          printf("[%d] Stopped ", i + 1);
        printf("%s [%d] \n", working_proc[i].pname, working_proc[i].pid);
      }
    }
  }

  return 0;
}
