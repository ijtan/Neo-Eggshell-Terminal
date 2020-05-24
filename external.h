#ifndef NEOEGGSHELL_EXTERNAL_H
#define NEOEGGSHELL_EXTERNAL_H

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include "ext/linenoise.h"
#include "internal.h"
#include "signalHandler.h"


using namespace std;

struct proc {
    pid_t pid;
    string name;
};

int runExt(vector<string> &argVector, int *conf);
//void vec2ptr(char**args, vector<string> &innie);
int statusChecker(int status, pid_t pid, string name);

class external;


#endif //NEOEGGSHELL_EXTERNAL_H
