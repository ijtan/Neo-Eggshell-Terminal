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
#include <memory>

using namespace std;
int internalHandler(string command, vector<string> argsV);
void echo(vector<string>args);
int better_set(string variable,string value);
void better_unset(string variable);
void changeDirs(vector<string> args);
int sourceStart(vector<string> args);
int internalChecker(string command);
int internalHandlerNoCHild(string command, vector<string> argsV);
void freeVars();
void printVarVec();

class internal;


#endif //NEOEGGSHELL_INTERNAL_H
