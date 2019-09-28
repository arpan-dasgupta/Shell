#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <string.h>
#include <signal.h>
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

char *trimwhitespace10(char *str) {
  char *end;
  while (isspace((unsigned char)*str)) str++;
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) end--;
  end[1] = '\0';
  return str;
}

char *getStatusID(int id) {
  char *pid = (char *)malloc(sizeof(char) * 100);
  sprintf(pid, "%d", id);
  // printf("%d", id);
  // printf("%s\n", pid);
  char ad1[100] = "/proc/", ad2[100] = "/proc/", ad3[100] = "/proc/";
  for (int i = 0; i < strlen(pid); i++) {
    ad3[i + 6] = pid[i];
  }
  {
    ad3[strlen(pid) + 6] = '/';
    ad3[strlen(pid) + 7] = 's';
    ad3[strlen(pid) + 8] = 't';
    ad3[strlen(pid) + 9] = 'a';
    ad3[strlen(pid) + 10] = 't';
    ad3[strlen(pid) + 11] = 'u';
    ad3[strlen(pid) + 12] = 's';
    ad3[strlen(pid) + 13] = '\0';
  }
  char pinf[1000000];
  int x = 0;
  {
    FILE *status = fopen(ad3, "r");
    while (1) {
      char c = fgetc(status);
      if (feof(status)) {
        break;
      }
      pinf[x++] = c;
    }
    fclose(status);
  }
  pinf[x] = '\n';
  // printf("%s ", pinf);
  char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
  char *token;
  char token2[1024];
  char *saveptr1, *saveptr2;
  int j, i;

  for (j = 1, str1 = pinf;; j++, str1 = NULL) {
    token = strtok_r(str1, "\n", &saveptr1);
    if (token == NULL) break;
    token = trimwhitespace10(token);
    // printf("%s\n", token);
    for (i = 0; i <= strlen(token); i++) token2[i] = token[i];
    subtoken = strtok_r(token, " \t", &saveptr2);
    subtoken = trimwhitespace10(subtoken);
    if (subtoken == NULL)
      continue;
    else if (strcmp(subtoken, "State:") == 0) {
      printf("%s ", token2);
    }
  }
}

void sigint(int sn) {
  // printf("ok");
  if (curPID > 0 && curPID != getpid()) {
    raise(SIGINT);
  }
}
void sigstop(int sn) {
  if (curPID > 0 && curPID != getpid()) raise(SIGTSTP);
}

int main(int argc, char const *argv[]) {
  signal(SIGINT, sigint);
  signal(SIGTSTP, sigstop);
  Proccount = 0;
  marker = 0;
  username = getenv("USER");
  initialise(username);
  getcwd(home, sizeof(home));
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
    if (marker) {
      for (int i = 0; i < Proccount; i++) {
        if (status[i] == 0) continue;
        printf("[%d] ", i + 1);
        getStatusID(working_proc[i].pid);
        printf("%s [%d] \n", working_proc[i].pname, working_proc[i].pid);
      }
      marker = 0;
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
      marker = 1;
    }
  }

  return 0;
}
