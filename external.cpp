#include <cstdio>
#include "unistd.h"
#include "external.h"

void runExt(char * exec){
    pid_t pid = fork();
    printf("%d\n",pid);
    printf("GET %d\n",getpid());

    printf("-------------------\n");
    if(pid == 0){
        std::cout << pid  << ": exiting\n";
        exit(0);
    }

    char *arg[2];
    arg[0] = "/bin/ls";
    arg[1] = NULL;
    execv(arg[0], arg);
}