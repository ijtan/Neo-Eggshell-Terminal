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
int internalHandler(std::string command, std::vector<std::string>argsV);
void echo(std::vector<std::string>args);
#define MaxCmdLen 255
#define MAX_INTERNAL_CMDS 100
class internal {

};


#endif //NEOEGGSHELL_INTERNAL_H
