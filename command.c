#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include "list.h"
#include "pwd.h"
#include "echo.h"
#include "cd.h"
#include "others.h"
#include "pinfo.h"
#include "history.h"
#include "nightswatch.h"
#include "setenv.h"
#include "globalVar.h"

#define makeblue printf("\033[1;34m")
#define makedef printf("\033[0m")
#define clear() printf("\033[H\033[J")

struct comm getProcess(int id) {
  struct comm false;
  if (id < 1 || id > Proccount) {
    printf("Invalid job ID");
    false.status = -1;
    return false;
  }
  return working_proc[id - 1];
}

struct comm *runCommand(char *, char[]);
// struct comm runCommand1(char[], char *);

char *trimwhitespace(char *str) {
  char *end;
  while (isspace((unsigned char)*str)) str++;
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end)) end--;
  end[1] = '\0';
  return str;
}

int stringToInt(char *s) {
  int sum = 0;
  for (int i = 0; i < strlen(s); i++) {
    sum *= 10;
    sum += (s[i] - '0');
  }
  // printf("%d ", sum);
  return sum;
}

struct comm chooseCommand(char home[], char *str) {
  char *str1, *str2, *str3, *subtoken, *subtoken1, *subtoken2, *subtoken3;
  char *token;
  char *saveptr1, *saveptr2, *saveptr3;
  int j, i, retcount = 0;
  int temp[1024];
  char *t2[1024];
  int fl = 0;

  int orgIn = dup(STDIN_FILENO), orgOut = dup(STDOUT_FILENO);
  int jobs = 0;
  for (j = 1, str1 = str;; j++, str1 = NULL) {
    token = strtok_r(str1, ";", &saveptr1);
    if (token == NULL) break;
    token = trimwhitespace(token);
    char *tempstr = (char *)malloc(sizeof(char) * strlen(token));
    strcpy(tempstr, token);
    token = strtok(token, "<>");
    // printf("%s ", token);
    // printf("%d: %s\n", j, tempstr);
    subtoken = strtok_r(token, " \t", &saveptr2);
    if (subtoken == NULL) continue;
    int hh;
    // printf("%d: %s\n", j, tempstr);
    for (hh = 1, str3 = tempstr;; hh++, str3 = NULL) {
      char *tempstring = strtok_r(str3, " \t", &saveptr3);
      if (tempstring == NULL) break;
      tempstring = trimwhitespace(tempstring);
      // printf("%d: %s ++ \n", hh, tempstring);
      if (strcmp(tempstring, ">") == 0) {
        str3 = NULL;
        char *tempstring = strtok_r(str3, " \t", &saveptr3);
        tempstring = trimwhitespace(tempstring);
        if (tempstring == NULL) {
          printf("Invalid redirection\n");
          break;
        }
        int file_d = open(tempstring, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(file_d, 1);
      } else if (strcmp(tempstring, ">>") == 0) {
        str3 = NULL;
        char *tempstring = strtok_r(str3, " \t", &saveptr3);
        tempstring = trimwhitespace(tempstring);
        if (tempstring == NULL) {
          printf("Invalid redirection\n");
          break;
        }
        int file_d = open(tempstring, O_WRONLY | O_CREAT | O_APPEND, 0644);
        dup2(file_d, 1);
      } else if (strcmp(tempstring, "<") == 0) {
        str3 = NULL;
        char *tempstring = strtok_r(str3, " \t", &saveptr3);
        tempstring = trimwhitespace(tempstring);
        int file_d = open(tempstring, O_RDONLY, 0644);
        if (file_d == -1) {
          perror("Unable to Open File: ");
          exit(0);
        }
        if (tempstring == NULL) {
          printf("Invalid redirection\n");
          break;
        }
        dup2(file_d, 0);
      }
    }
    // printf("%s \n", subtoken);
    if (strcmp(subtoken, "pwd") == 0) {
      pwd();
      continue;
    } else if (strcmp(subtoken, "echo") == 0) {
      subtoken = strtok_r(NULL, "", &saveptr2);
      if (subtoken == NULL) subtoken = "";
      echo(subtoken);
      printf("\n");
      continue;
    } else if (strcmp(subtoken, "cd") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL) subtoken = "";
      cd(home, subtoken);
      continue;
    } else if (strcmp(subtoken, "ls") == 0) {
      int l_flag = 0, a_flag = 0;
      char *dir = "";
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL) subtoken = "";
      subtoken1 = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken1 == NULL) subtoken1 = "";
      subtoken2 = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken2 == NULL) subtoken2 = "";
      if (strlen(subtoken2) > 0)
        if (subtoken2[0] == '-') {
          if (strlen(subtoken2) == 2)
            if (subtoken2[1] == 'l')
              l_flag = 1;
            else
              a_flag = 1;
          else if (strlen(subtoken2) == 3) {
            l_flag = 1;
            a_flag = 1;
          }
        } else
          dir = subtoken2;
      if (strlen(subtoken1) > 0)
        if (subtoken1[0] == '-') {
          if (strlen(subtoken1) == 2)
            if (subtoken1[1] == 'l')
              l_flag = 1;
            else
              a_flag = 1;
          else if (strlen(subtoken1) == 3) {
            l_flag = 1;
            a_flag = 1;
          }
        } else
          dir = subtoken1;
      if (strlen(subtoken) > 0)
        if (subtoken[0] == '-') {
          if (strlen(subtoken) == 2)
            if (subtoken[1] == 'l')
              l_flag = 1;
            else
              a_flag = 1;
          else if (strlen(subtoken) == 3) {
            l_flag = 1;
            a_flag = 1;
          }
        } else
          dir = subtoken;
      listfiles(home, dir, l_flag, a_flag);
    } else if (strcmp(subtoken, "exit") == 0 || strcmp(subtoken, "q") == 0 ||
               strcmp(subtoken, "quit") == 0) {
      exit(0);
    } else if (strcmp(subtoken, "clear") == 0) {
      clear();
    } else if (strcmp(subtoken, "pinfo") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL)
        pinfo(0, "");
      else {
        pinfo(1, trimwhitespace(subtoken));
      }
    } else if (strcmp(subtoken, "history") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL)
        histDisp("10");
      else {
        histDisp(subtoken);
      }
    } else if (strcmp(subtoken, "setenv") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL) {
        printf("Invalid args (1/2 required)\n");
      } else {
        subtoken1 = strtok_r(NULL, " \t", &saveptr2);
        subtoken2 = strtok_r(NULL, " \t", &saveptr2);
        if (subtoken2 != NULL) {
          printf("Invalid args (1/2 required)\n");
        } else
          setenviron(subtoken, subtoken1);
      }
    } else if (strcmp(subtoken, "unsetenv") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL) {
        printf("Invalid args (1 required)\n");
      } else {
        subtoken1 = strtok_r(NULL, " \t", &saveptr2);
        if (subtoken1 != NULL) {
          printf("Invalid args (1 required)\n");
        } else
          unsetenviron(subtoken);
      }
    } else if (strcmp(subtoken, "nightswatch") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      subtoken1 = strtok_r(NULL, " \t", &saveptr2);
      subtoken2 = strtok_r(NULL, " \t", &saveptr2);
      subtoken = trimwhitespace(subtoken);
      subtoken1 = trimwhitespace(subtoken1);
      subtoken2 = trimwhitespace(subtoken2);
      if (strcmp(subtoken, "-n") != 0) {
        printf("Invalid command\n");
      }
      if (strcmp(subtoken2, "interrupt") == 0)
        nightint(subtoken1);
      else if (strcmp(subtoken2, "dirty") == 0)
        nightdirt(subtoken1);
      else {
        printf("Invalid command\n");
      }
    } else if (strcmp(subtoken, "jobs") == 0) {
      jobs = 1;
    } else if (strcmp(subtoken, "kjob") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      subtoken1 = strtok_r(NULL, " \t", &saveptr2);
      subtoken = trimwhitespace(subtoken);
      subtoken1 = trimwhitespace(subtoken1);
      if (subtoken == NULL || subtoken1 == NULL) {
        printf("Incorect arguments\n");
        continue;
      }
      int id = stringToInt(subtoken);
      int sig = stringToInt(subtoken1);
      struct comm job = getProcess(id);
      if (job.status != -1) {
        if (status[id - 1] == 0) {
          printf("Job had terminated already\n");
        } else {
          kill(job.pid, sig);
          status[id - 1] = 0;
          printf("Process %s with PID - %d killed\n",
                 working_proc[id - 1].pname, working_proc[id - 1].pid);
        }
      }
      // printf("%d %d\n", id, sig);
    } else if (strcmp(subtoken, "overkill") == 0) {
      for (int i = 0; i < Proccount; i++) {
        struct comm job = getProcess(i + 1);
        if (job.status != -1) {
          if (status[i] == 0) {
            // printf("Job had terminated already");
          } else {
            kill(job.pid, 9);
            status[i] = 0;
            printf("Process %s with PID - %d killed\n", working_proc[i].pname,
                   working_proc[i].pid);
          }
        }
      }
    } else if (strcmp(subtoken, "fg") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL) {
        printf("Required 1 argument\n");
        continue;
      }
      subtoken = trimwhitespace(subtoken);
      int id = stringToInt(subtoken);
      struct comm job = getProcess(id);
      if (job.status == -1) continue;
      if (status[id - 1] == 0) {
        printf("Job had terminated already\n");
      } else {
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
        tcsetpgrp(0, job.pid);
        kill(job.pid, SIGCONT);

        int st;
        waitpid(job.pid, &st, WUNTRACED);
        tcsetpgrp(0, getpgrp());
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);

        printf("[%d] %d continued\n", id, job.pid);
        status[id - 1] = 0;

        // similar to that of process.c, if we get ctrl z
        if (WIFSTOPPED(st)) {
          working_proc[Proccount] = job;
          Proccount++;
          status[Proccount - 1] = 1;
        }
      }
    } else if (strcmp(subtoken, "bg") == 0) {
      subtoken = strtok_r(NULL, " \t", &saveptr2);
      if (subtoken == NULL) {
        printf("Required 1 argument\n");
        continue;
      }
      subtoken = trimwhitespace(subtoken);
      int id = stringToInt(subtoken);
      struct comm job = getProcess(id);
      if (job.status == -1) continue;
      if (status[id - 1] == 0) {
        printf("Job had terminated already\n");
      } else {
      }
    } else if (subtoken[0] == 27) {
      int up = 0;
      for (int i = 2; i < strlen(subtoken); i += 3) {
        if (subtoken[i] == 'A') up++;
      }
      up = (up >= 10) ? 10 : up;
      char *pcom = upArr(up);
      // printf("%c \n", subtoken[2]);
      // continue;
      // printf("%s ", pcom);
      // runCommand(home, pcom);
      makeblue;
      printf("<%s@%s:%s> >> ", username, sys_name, curdir);
      makedef;
      printf("%s\n", pcom);
      struct comm *a = runCommand(home, pcom);
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
    } else {
      char *subt[100];
      // char *ttt;
      // char *temporary;
      // ttt = (char *)realloc(ttt, strlen(ttt) + strlen(subtoken));
      // strcat(ttt, subtoken);
      // printf("%s ", ttt);
      subt[0] = subtoken;
      subt[0] = trimwhitespace(subt[0]);
      int i = 1;
      while (i < 100) {
        subt[i] = strtok_r(NULL, " \t", &saveptr2);
        if (subt[i] == NULL) break;
        subt[i] = trimwhitespace(subt[i]);
        if (strcmp(subt[i], "&") == 0) {
          fl = 1;
          i--;
        }
        // else
        // {
        //     // strcat(ttt, subt[i]);
        // }
        i++;
      }
      int x = otherCommands(subtoken, subt, fl);
      // char *ok;
      if (fl == 1 && x != -1) {
        int totlen = 0;
        for (int k = 0; k < i; k++) {
          totlen += strlen(subt[k]) + 1;
        }
        t2[retcount] = (char *)malloc(sizeof(char) * totlen);
        t2[retcount][0] = '\0';
        for (int k = 0; k < i; k++) {
          strcat(t2[retcount], subt[k]);
          strcat(t2[retcount], " ");
        }
        temp[retcount++] = x;
      }
    }
  }
  dup2(orgIn, STDIN_FILENO);
  dup2(orgOut, STDOUT_FILENO);

  struct comm ans;
  ans.jobs = jobs;
  //    =      (struct comm *)malloc(sizeof(struct comm) * (retcount + 1));
  //    ans.pid = retcount;
  //    for (i = 0; i < retcount; i++) {
  ans.pid = temp[0];
  if (retcount > 0) {
    for (j = 0; j < strlen(t2[0]); j++) {
      ans.pname[j] = t2[0][j];
    }
  }
  ans.status = (retcount > 0);
  // printf("%d ", ans.status);
  return ans;
}

struct comm runCommand1(char home[], char *cmd) {
  char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
  char *token;
  char *saveptr1, *saveptr2;
  int j, i;
  int numcom = 0;
  struct comm returned;
  returned.pid = 0;
  returned.status = 0;
  str2 = cmd;
  char *tmp = (char *)malloc(strlen(cmd) * sizeof(char));
  strcpy(tmp, cmd);
  // printf("%s ++ %d\n", cmd, numcom);
  for (j = 1, str1 = tmp;; j++, str1 = NULL) {
    token = strtok_r(str1, "|", &saveptr1);
    if (token == NULL) break;
    token = trimwhitespace(token);
    // printf("%s \n", token);
    numcom++;
  }
  if (numcom == 1) {
    returned = chooseCommand(home, cmd);
    return returned;
  }
  int orgIn = dup(STDIN_FILENO), orgOut = dup(STDOUT_FILENO);
  int jb = 0;
  int fd[2][2], counter = 0;
  // printf("%s ++ %d\n", cmd, numcom);
  for (j = 1, str1 = cmd;; j++, str1 = NULL) {
    token = strtok_r(str1, "|", &saveptr2);
    // printf("%s \n", token);
    if (token == NULL) break;
    token = trimwhitespace(token);
    // printf("%s \n", token);
    // numcom++;
    if (counter % 2 != 0) {
      pipe(fd[0]);
    } else {
      pipe(fd[1]);
    }
    int cpid = fork();

    if (cpid == -1) {
      perror("Couldn't fork");
    }
    if (cpid == 0) {
      if (counter == 0) {
        dup2(fd[1][1], STDOUT_FILENO);
      } else if (counter == numcom - 1) {
        if (numcom % 2 != 0) {
          dup2(fd[0][0], STDIN_FILENO);
        } else {
          dup2(fd[1][0], STDIN_FILENO);
        }

      } else {
        if (counter % 2 != 0) {
          dup2(fd[1][0], STDIN_FILENO);
          dup2(fd[0][1], STDOUT_FILENO);
        } else {
          dup2(fd[0][0], STDIN_FILENO);
          dup2(fd[1][1], STDOUT_FILENO);
        }
      }

      // int pid = fork();

      // if(execvp(home, token)){

      // }
      returned = chooseCommand(home, token);
      exit(0);
    } else {
      if (counter == 0) {
        close(fd[1][1]);
      } else if (counter == numcom - 1) {
        if (numcom % 2 != 0) {
          close(fd[0][0]);
        } else {
          close(fd[1][0]);
        }
      } else {
        if (counter % 2 != 0) {
          close(fd[1][0]);
          close(fd[0][1]);
        } else {
          close(fd[0][0]);
          close(fd[1][1]);
        }
      }
    }
    waitpid(cpid, NULL, 0);
    counter++;
    // printf("%d ", returned.status);
    if (returned.jobs == 1) jb = 1;
    if (returned.status == 1) {
      return returned;
    }
  }
  if (jb) returned.jobs = 1;
  dup2(orgIn, STDIN_FILENO);
  dup2(orgOut, STDOUT_FILENO);
  return returned;
}

struct comm *runCommand(char home[], char *cmd) {
  char *str1, *str2, *subtoken, *subtoken1, *subtoken2;
  char *token;
  char *saveptr1, *saveptr2;
  int j, i;
  int retcount = 0;
  int numcom = 0;
  histAdd(home, cmd);
  struct comm ret, tempt[1000];
  char temp[1000];
  int jb = 0;
  for (j = 1, str1 = cmd;; j++, str1 = NULL) {
    token = strtok_r(str1, ";", &saveptr1);
    if (token == NULL) break;
    token = trimwhitespace(token);
    // printf("%s \n", token);
    numcom++;
    ret = runCommand1(home, token);
    if (ret.jobs == 1) jb = 1;
    if (ret.status == 1) {
      tempt[retcount] = ret;
      retcount++;
    }
  }
  struct comm *returned =
      (struct comm *)malloc(sizeof(struct comm) * (retcount + 1));
  returned[0].pid = retcount;
  returned[0].jobs = jb;
  for (int i = 1; i <= retcount; i++) {
    returned[i] = tempt[i - 1];
  }
  return returned;
}