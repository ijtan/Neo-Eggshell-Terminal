//
// Created on 5/20/2020.
//

#ifndef NEOEGGSHELL_VARIABLEHANDLER_H
#define NEOEGGSHELL_VARIABLEHANDLER_H

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <cctype>
#include "external.h"

void printVars();
void initVars();
std::vector<std::string> getVars();
char ** parseVars(char **input);

class variableHandler {

};


#endif //NEOEGGSHELL_VARIABLEHANDLER_H
