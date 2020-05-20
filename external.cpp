#include "external.h"
using namespace std;
int runExt(vector<string> argVector){
    // set last token to NULL
     char *args[argVector.size()+1];
    int i = 0;
    for(const auto& str:argVector){
        args[i] = const_cast<char *>(str.c_str());
        i++;
    }
    args[i]=(NULL);
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

string runExtRedir(vector<string> argVector){
    // set last token to NULL
    char **args;
    int i = 0;
    for(const auto& str:argVector){
        args[i] = const_cast<char *>(str.c_str());
        i++;
    }
    args[i]=(NULL);
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