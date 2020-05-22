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
extern vector<pid_t> StpProcs;
int runExt(vector<string>& argVector, int *conf);
int statusChecker(int status, pid_t pid);
class external;


#endif //NEOEGGSHELL_EXTERNAL_H
