#ifndef NEOEGGSHELL_REDIRECTION_H
#define NEOEGGSHELL_REDIRECTION_H
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

#include "Executor.h"
#include "reparse.h"

void pipeTester();

using namespace std;
int truncOut(char* filename);
int input(char* filename);
int append(char* filename);
void BetterSourceRun(vector<string> args);

struct needWaits {
    int returnCode;
    int PipeCount;
    vector<string> newArgV;
    pid_t PID;
};
vector<needWaits> initPipes(vector<string> argV);
int InitialzeRedir(vector<int> conf, vector<string>& args);
void flagger(string line, vector<int>& RedirectConfig);
#endif  //NEOEGGSHELL_REDIRECTION_H