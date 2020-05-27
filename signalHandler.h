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
#include "Executor.h"

void neoSigHand(int signum);
sig_t sigHandInstaller(int signum);
void resumeStopped();
#endif //NEOEGGSHELL_SIGNALHANDLER_H
