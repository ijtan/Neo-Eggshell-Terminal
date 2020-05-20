

#ifndef NEOEGGSHELL_EXTERNAL_H
#define NEOEGGSHELL_EXTERNAL_H
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
int runExt(std::vector<std::string> argVector);
std::string runExtRedir(std::vector<std::string> argVector);
class external {

};


#endif //NEOEGGSHELL_EXTERNAL_H
