#include <cstdio>
#include "unistd.h"
#include <sys/types.h>
#include <sys/wait.h>
#include "external.h"
using namespace std;
void runExt(char * exec){
    pid_t pid = fork();
    printf("%d\n",pid);
    printf("GET %d\n",getpid());

    printf("-------------------\n");
//    if(pid == 0){
//        std::cout << pid  << ": exiting\n";
//        exit(0);
//    }
    if(pid == 0) {
        puts("executing");
        char *arg[2];
        arg[0] = "/bin/ls";
        cout << execlp("ls","ls", NULL)<< ": execv\n";
    }
    int status;
    puts("Waiting");
    waitpid(-1, &status, 0);
    puts("Done");
    std::cout << pid  << " EXITING";
}