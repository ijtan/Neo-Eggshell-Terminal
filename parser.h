//
// Created on 5/19/2020.
//

#ifndef NEOEGGSHELL_PARSER_H
#define NEOEGGSHELL_PARSER_H

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include "internal.h"
#include "variableHandler.h"
int parseLine(std::string line, char **input);

#endif //NEOEGGSHELL_PARSER_H
