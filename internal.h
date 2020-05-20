//
// Created on 5/20/2020.
//

#ifndef NEOEGGSHELL_INTERNAL_H
#define NEOEGGSHELL_INTERNAL_H


#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include "variableHandler.h"
using namespace std;
int internalHandler(string command, vector<string>argsV);
void echo(vector<string>args);
void unset(vector<string> args);
#define MaxCmdLen 255
#define MAX_INTERNAL_CMDS 100
class internal {

};


#endif //NEOEGGSHELL_INTERNAL_H
