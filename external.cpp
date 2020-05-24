#include "external.h"

using namespace std;
vector<proc> StpProcs;

int runExt(vector<string> &argVector, int *conf) {
    // set last token to NULL

    char *args[argVector.size() + 1];
    int i = 0;
    for (const auto &str : argVector) {
        args[i] = const_cast<char *>(str.c_str());
        i++;

        args[i] = (NULL);
    }

    //0: >>
    //1: >
    //2: <
    //3: |
    //4: & -> background run
    int waitOpt = 0;

    if (conf[4] == 1)
        waitOpt = WNOHANG;

    pid_t pid = fork();
    signal(SIGINT, sigHandler);
    if (pid == 0) {
        if (conf[3] == 1)
            if (strcmp(*initPipes(conf, argVector), "-5") == 0)
                return -5;
        InitialzeRedir(conf, argVector);
    }

    string tmp(args[0]);
    vector<string> argsVec;
    for (auto a : args) {
        if (a == NULL)
            break;
        string tmp(a);
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
    if(waitOpt == 0)
        return statusChecker(status, pid, argVector[0]);
    else
        return 0;
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