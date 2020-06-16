//
// Created on 5/20/2020.
//

#ifndef NEOEGGSHELL_VARIABLEHANDLER_H
#define NEOEGGSHELL_VARIABLEHANDLER_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

#include "Executor.h"

void printVars();
using namespace std;
vector<string> parseVars(vector<string> inputVector);
void initVars(vector<string> &env);
//void stringCombiner(vector<string> &input, string &line);
class variableHandler {
};

#endif  //NEOEGGSHELL_VARIABLEHANDLER_H
