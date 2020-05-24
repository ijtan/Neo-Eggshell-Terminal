#include "external.h"

using namespace std;
vector<proc> StpProcs;

int runExt(vector<string> &argVector, int *conf) {
    int waitOpt = 0;

    char **args = (char**)calloc(argVector.size() + 1, sizeof(char));
    int i = 0;
    cout << "initial strncpy" << endl;
    for(auto arg:argVector){
        cout << ">"<<arg << endl;
        strncpy(args[i], arg.c_str(), sizeof(args[i]));
        i++;
    }
    cout << "done" << endl;
    if (conf[4] == 1)
        waitOpt = WNOHANG;

    
    pid_t pid = fork();
    if(pid==-1){
        perror("fork");
        return -5;
    }
    signal(SIGINT, sigHandler);
    if (pid == 0) {
        if (conf[3] == 1) {
            cout << "pipe init" << endl;
            auto feedback = initPipes(conf, argVector, args);
            if(strcmp(feedback[0], "-5")==0){return -5;}
        }
        cout << "redir checker" << endl;
        InitialzeRedir(conf, args, argVector.size());

        cout << "checking internals" << endl;
        string tmp(args[0]);
        vector<string> argsVec;
        for (int i = 0 ; i<argVector.size(),args[i]!=NULL;i++) {
            string tmp(args[i]);
            cout << "internal check arg: " << tmp << endl;
            argsVec.push_back(tmp);
        }

        cout << "checking if internals " << endl;
        if (internalHandler(argsVec[0], argsVec) == 0)
            //checks if the internalHandler matched; meaning that an internal command was run and we do not need further execution
            return -5;
        cout << "execing " << endl;
        int code = execvp(args[0], args);
        if (code == -1) {
            perror("Execution");
            return -5;
        }

        int status;
        waitpid(pid, &status, waitOpt);
        if (waitOpt == 0)
            return statusChecker(status, pid, argVector[0]);
        else
            return 0;
    }
    free(args);
}

int statusChecker(int status, pid_t pid, string name) {
    if (WIFSIGNALED(status)) {
        cout << "Signal Code " << WTERMSIG(status) << ":" << sys_siglist[WTERMSIG(status)] << endl;
        if (WTERMSIG(status) == 19)
            StpProcs.push_back(proc{pid, move(name)});

        return -1;
    }
    if (WIFEXITED(status)) {
        char envStr[sizeof(WEXITSTATUS(status)) + 10];
        sprintf(envStr, "EXITCODE=%d", (WEXITSTATUS(status)));
        vector<string> env;
        env.emplace_back(envStr);
        set(env);
        return 0;
    }
    return 0;
}

vector<proc2> getStpProcs() {
    vector<proc2> procs2;
    for (auto prc : StpProcs) {
        proc2 tmp{prc.pid, prc.name};
        procs2.push_back(tmp);
    }
    return procs2;
}
