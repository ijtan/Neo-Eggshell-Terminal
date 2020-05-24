#ifndef NEOEGGSHELL_REDIRECTION_H
#define NEOEGGSHELL_REDIRECTION_H
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include <iostream>
#include "external.h"
#include "reparse.h"
void pipeTester();

using namespace std;
int truncOut(char* filename);
int input(char* filename);
int append(char* filename);
void sourceRun(string filename);

char ** initPipes(int *conf, vector<string> argV, char ** args);
int InitialzeRedir(int* conf, char** args, int size)  ;
#endif //NEOEGGSHELL_REDIRECTION_H