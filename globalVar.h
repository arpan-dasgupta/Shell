struct comm {
  int pid;
  char pname[1000];
  int status;
  int jobs;
} working_proc[2048];

int status[2048];
int Proccount;
int curPID;
int marker;
char *username;
char home[1024];
char sys_name[102];
char curdir[1024];