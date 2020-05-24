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
#include "signalHandler.h"
#include "redirection.h"
#include "externalStpProcs.h"

using namespace std;
int internalHandler(string command, vector<string>argsV);
void echo(vector<string>args);
int set(vector<string> args);
int better_set(string variable,string value);
void unset(vector<string> args);
void changeDirs(vector<string> args);
void sourceStart(vector<string> args);

void freeVars();
void printVarVec();

class internal;


#endif //NEOEGGSHELL_INTERNAL_H
