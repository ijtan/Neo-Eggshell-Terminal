#ifndef NEOEGGSHELL_REDIRECTION_H
#define NEOEGGSHELL_REDIRECTION_H
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include "external.h"
#include "reparse.h"

void pipeTester();

using namespace std;
int truncOut(char* filename);
int input(char* filename);
int append(char* filename);
void BetterSourceRun(string filename);
vector<string> initPipes(vector<string> argV, vector<pid_t> &toWait);
int InitialzeRedir(vector<int> conf, vector<string>& args) ;
void flagger(string line, vector<int>&RedirectConfig);
#endif //NEOEGGSHELL_REDIRECTION_H