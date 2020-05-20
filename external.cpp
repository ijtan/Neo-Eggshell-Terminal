#include "external.h"
using namespace std;
int runExt(char **args){
    pid_t pid = fork();
    if(pid == 0) {
        int code = execvp(args[0],args);
        if(code == -1){
            perror("Execution");
            exit(-1);
        }
        exit(0);
    }
    int status;
    waitpid(-1, &status, 0);

    if(WIFEXITED(status))
        cout << "exited status: " << WEXITSTATUS(status) << endl;
    if(WIFSIGNALED(status)){
        cout << "exited with signal: " << WTERMSIG(status) << endl;
    }
    return 0;
}

string runExtRedir(char **args){
    pid_t pid = fork();
    if(pid == 0) {
        int code = execvp(args[0],args);
        if(code == -1){
            perror("Execution");
            exit(-1);
        }
        exit(0);
    }
    int status;
    waitpid(-1, &status, 0);

    if(WIFEXITED(status))
        cout << "exited status: " << WEXITSTATUS(status) << endl;
    if(WIFSIGNALED(status)){
        cout << "exited with signal: " << WTERMSIG(status) << endl;
    }
    return 0;
}