#ifndef NEOEGGSHELL_EXTERNAL_H
#define NEOEGGSHELL_EXTERNAL_H
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include "linenoise.h"
int runExt(std::vector<std::string> argVector);
int runExtRedir(std::vector<std::string> argVector, char* buf, size_t size) ;
class external {

};


#endif //NEOEGGSHELL_EXTERNAL_H
