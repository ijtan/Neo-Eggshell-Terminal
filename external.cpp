#include <cstdio>
#include "unistd.h"
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
        cout << execl("/bin/ls","ls", NULL)<< ": execv\n";
    }
    puts("Done");
    std::cout << pid  << " EXITING";
    sleep(2);
}