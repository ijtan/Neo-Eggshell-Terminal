#include <cstdio>
#include "unistd.h"
#include "external.h"

void runExt(char * exec){
    pid_t pid = fork();
    printf("%d\n",pid);
    printf("GET %d\n",getpid());
}