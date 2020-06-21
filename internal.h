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

#include "StoppedHelper.h"
#include "redirection.h"
#include "variableHandler.h"

using namespace std;
int internalHandler(string command, vector<string> argsV);
void echo(vector<string> args);

void changeDirs(vector<string> args);
void sourceStart(vector<string> args);
int internalChecker(string command);
int internalHandlerNoCHild(string command, vector<string> argsV);

void listProc();

#endif  //NEOEGGSHELL_INTERNAL_H
