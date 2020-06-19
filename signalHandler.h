//
// Created on 5/21/2020.
//

#ifndef NEOEGGSHELL_SIGNALHANDLER_H
#define NEOEGGSHELL_SIGNALHANDLER_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include "StoppedHelper.h"

using namespace std;


void neoSigHand(int signum);
sig_t sigHandInstaller(int signum);
void resumeStopped();
#endif  //NEOEGGSHELL_SIGNALHANDLER_H
