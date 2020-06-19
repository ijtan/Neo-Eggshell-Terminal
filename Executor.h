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

int Executor(vector<string> &argVector, vector<int> conf);
int statusChecker(int status, pid_t pid, string name);

class external;

#endif  //NEOEGGSHELL_EXTERNAL_H
