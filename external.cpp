#include "external.h"
using namespace std;
void runExt(char **args){
    pid_t pid = fork();
    if(pid == 0) {
        int code = execvp(args[0],args);
        if(code == -1){
            cout << code << ": execv\n"<<endl;
            perror("EXECV, exiting!");
            exit(-1);
        }
    }
    int status;
    waitpid(-1, &status, 0);
}