#include "external.h"
using namespace std;
int runExt(char **args){
    pid_t pid = fork();
    if(pid == 0) {
        printf("pre exec");
        int code = execvp(args[0],args);
        printf("post exec");
        if(code == -1){
            perror("Execution");
            exit(-1);
        }
        exit(0);
    }
    int status;
    printf("pre wait");
    waitpid(-1, &status, 0);
    printf("post wait");

    if(WIFEXITED(status))
        cout << "exited status: " << WEXITSTATUS(status) << endl;
    if(WIFSIGNALED(status)){
        cout << "exited with signal: " << WTERMSIG(status) << endl;
    }
    return 0;
}