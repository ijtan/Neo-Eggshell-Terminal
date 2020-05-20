#include "external.h"
using namespace std;
void runExt(char *exec){
    pid_t pid = fork();
    if(pid == 0) {
        char *argv[2];
        argv[0] = exec;
        argv[1] = NULL;
        int code = execvp(exec,argv);
        cout << code << ": execv\n"<<endl;
        if(code == -1){
            puts("Error encountered, exiting!");
            exit(-1);
        }
    }

    int status;
    puts("Waiting");
    waitpid(-1, &status, 0);
    puts("Done");
    cout << pid  << " EXITING";
}