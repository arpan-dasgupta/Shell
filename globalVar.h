struct comm {
  int pid;
  char pname[1000];
  int status;
  int jobs;
} working_proc[2048];

int status[2048];
int Proccount;
int curPID;