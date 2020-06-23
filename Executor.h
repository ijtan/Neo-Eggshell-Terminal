#ifndef NEOEGGSHELL_EXTERNAL_H
#define NEOEGGSHELL_EXTERNAL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

#include "ext/linenoise.h"
#include "internal.h"
#include "signalHandler.h"

struct proc {
     string name;
    pid_t pid;
};
int Executor(vector<string> &argVector, string &line, vector<int> conf);
int statusChecker(int status, pid_t pid, string name);

#endif  //NEOEGGSHELL_EXTERNAL_H
