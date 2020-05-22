//
// Created on 5/21/2020.
//

#ifndef NEOEGGSHELL_SIGNALHANDLER_H
#define NEOEGGSHELL_SIGNALHANDLER_H
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include "external.h"
void sigHandler(int signum);
struct proc2{
    pid_t pid;
    std::string name;
};
std::vector<proc2> getProcs();
#endif //NEOEGGSHELL_SIGNALHANDLER_H
