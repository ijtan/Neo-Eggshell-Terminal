//
// Created on 5/19/2020.
//

#ifndef NEOEGGSHELL_PARSER_H
#define NEOEGGSHELL_PARSER_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "Executor.h"
#include "internal.h"
#include "redirection.h"
#include "variableHandler.h"
using namespace std;
int parseLine(string line, vector<string> input);
int tokenize(char *line, char *copy, vector<string> &args);
void stringCombiner(vector<string> &input, string &line);
#endif  //NEOEGGSHELL_PARSER_H
