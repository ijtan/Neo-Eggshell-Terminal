#include "external.h"

using namespace std;
vector<proc> StpProcs;

int runExt(vector<string> &argVector, int *conf) {
    int waitOpt = 0;

    //char **args = (char**)calloc(argVector.size() + 1, sizeof(char));
    // int i = 0;
    // cout << "initial strncpy" << endl;
    // for(auto arg:argVector){
    //     cout << ">"<<arg << endl;
    //     strncpy(args[i], arg.c_str(), sizeof(args[i]));
    //     i++;
    // }
    if (conf[4] == 1)
        waitOpt = WNOHANG;

    // pid_t pid = fork();
    // if (pid == -1) {
    //     perror("fork");
    //     return -5;
    //}
    signal(SIGINT, sigHandler);
    pid_t mainPID = getpid();
    if (conf[3] == 1) {
        cout << "pipe init" << endl;
        cout << "main pid: " << mainPID<<endl;
        auto feedback = initPipes(argVector);
        if (getpid() != mainPID) {
            if (feedback[0].compare("-5") == 0) {
                return -5;
            } else {
                argVector.clear();
                argVector = feedback;
            }

            if (conf[0] == 1 || conf[1] == 1 || conf[2] == 1) {
                int redir = InitialzeRedir(conf, argVector);
            }

            if (internalHandler(argVector[0], argVector) == 0) {
                //checks if the internalHandler matched; meaning that an internal command was run and we do not need further execution
                return -5;
            }

            //converting vector into char* array since exec doesnt read vector
            char *args[argVector.size()];
            int i = 0;
            for (auto arg : argVector) {
                args[i] = strdup(arg.c_str());
                i++;
            }
            args[i] = NULL;

            int code = execvp(args[0], args);
            if (code == -1) {
                perror("Execution");
                cerr<<"its was: "<<args[0]<<"\n\n"<<endl;
                for(int j = 0; j< i;j++)
                    free(args[j]);
                return -5;
            }
        }

    } else {
        //main should be here if no piping ->  since yet we have no forks
        pid_t pid = fork();
        if (pid == -1) {
            perror("redirection fork");
        } else if (pid == 0) {
            if (conf[0] == 1 || conf[1] == 1 || conf[2] == 1) {
                cout << "redir init enters" << endl;
                int redir = InitialzeRedir(conf, argVector);
                if (redir != 0) {
                    return -5;
                }
            }
            if (internalHandler(argVector[0], argVector) == 0) {
                //checks if the internalHandler matched; meaning that an internal command was run and we do not need further execution
                return -5;
            }

            char *args[argVector.size()];
            int i = 0;
            for (auto arg : argVector) {
                cout << i << ":is now: " << (char *)arg.c_str() << endl;
                args[i] = strdup(arg.c_str());
                i++;
            }
            args[i] = NULL;
            cout << "execing :" << args[0] << "\n"
                 << endl;
            int code = execvp(args[0], args);
            if (code == -1) {
                perror("Execution");
                cout<<"np its was: "<<args[0]<<endl;
                for(int j = 0; j< i;j++)
                    free(args[j]);
                return -5;
            }
        }
    }

    //main should do this
    int status;
    pid_t wpid;
    cout << "waiting :" << getpid() << " should be same as:" << mainPID << endl;
    sleep(5);
    while ((wpid = wait(&status)) > 0) {
        if (waitOpt == 0)
            return statusChecker(status, wpid, argVector[0]);
        else
            return 0;
    }
}

// void vec2ptr(char **args, vector<string> &innie) {

// }

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
