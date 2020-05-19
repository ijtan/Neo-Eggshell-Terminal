#include "external.h"
using namespace std;
void runExt(char **exec){
    pid_t pid = fork();
    if(pid == 0) {
        cout << execlp(exec[0],exec[0],exec[1], NULL)<< ": execv\n";
    }

    int status;
    puts("Waiting");
    waitpid(-1, &status, 0);
    puts("Done");
    cout << pid  << " EXITING";
}