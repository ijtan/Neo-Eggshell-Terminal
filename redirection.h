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
#endif //NEOEGGSHELL_REDIRECTION_H