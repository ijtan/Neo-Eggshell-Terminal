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

struct proccess {
    string name;
    pid_t pid;
};

proccess getFirstProc();
void incrementProcs();
void addProc(string name, pid_t pid);
proccess getWaitingProc();
vector<proccess> getProcVec();

#endif  //NEOEGGSHELL_EXTERNALSTPPROCS_H
