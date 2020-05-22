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
void pipeTester();

using namespace std;
void  truncOut(string filename, string content);
FILE *append(char * lin, vector<string> &args);
string input(string filename);
#endif //NEOEGGSHELL_REDIRECTION_H