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
std::vector<pid_t> getProcs();
#endif //NEOEGGSHELL_SIGNALHANDLER_H
