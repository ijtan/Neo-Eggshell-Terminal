#ifndef NEOEGGSHELL_EXTERNALSTPPROCS_H
#define NEOEGGSHELL_EXTERNALSTPPROCS_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

struct process {
    string name;
    pid_t pid;
};

process getFirstProc();
void incrementProcs();
void addProc(string name, pid_t pid);
process getWaitingProc();
vector<process> getProcVec();

#endif  //NEOEGGSHELL_EXTERNALSTPPROCS_H
