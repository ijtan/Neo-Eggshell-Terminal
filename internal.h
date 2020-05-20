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
#include <cstring>

int internalHandler(char *command);
#define MaxCmdLen 100
#define MAX_INTERNAL_CMDS 100
void closeInternals();
class internal {

};


#endif //NEOEGGSHELL_INTERNAL_H
