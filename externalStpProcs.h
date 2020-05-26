//
// Created by kinkt on 23/05/2020.
//


#ifndef NEOEGGSHELL_EXTERNALSTPPROCS_H
#define NEOEGGSHELL_EXTERNALSTPPROCS_H
#include <unistd.h>
#include <cstring>
#include <vector>
#include <string>
#include <cstdio>
using namespace std;

struct proc2 {
    pid_t pid;
    string name;
};

vector<proc2> getStpProcs();
proc2 getCurrProc();
void pushProc(proc2 prc);
#endif //NEOEGGSHELL_EXTERNALSTPPROCS_H

