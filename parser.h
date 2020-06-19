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
int better_String_Combiner(vector<string> &input, string &line);
int multiCommands(string &line, vector<string> &input);
#endif  //NEOEGGSHELL_PARSER_H
