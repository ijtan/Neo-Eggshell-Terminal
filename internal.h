#ifndef NEOEGGSHELL_INTERNAL_H
#define NEOEGGSHELL_INTERNAL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "redirection.h"
#include "StoppedHelper.h"
#include "variableHandler.h"

using namespace std;
int internalHandler(string command, vector<string> argsV);
void echo(vector<string> args);
int better_set(string variable, string value);
void better_unset(string variable);
void changeDirs(vector<string> args);
void sourceStart(vector<string> args);
int internalChecker(string command);
int internalHandlerNoCHild(string command, vector<string> argsV);
void freeVars();
void printVarVec();

class internal;

#endif  //NEOEGGSHELL_INTERNAL_H
