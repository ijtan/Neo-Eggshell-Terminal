#include "external.h"

using namespace std;
vector<proc> StpProcs;

int Executer(vector<string> &argVector, vector<int>conf) {
    int waitOpt = 0;
    vector<pid_t> toWait;
    if (conf[4] == 1)
        waitOpt = WNOHANG;

    signal(SIGINT, sigHandler);
    pid_t mainPID = getpid();
    if (conf[3] == 1) {
        cout << "pipe init" << endl;
        cout << "main pid: " << mainPID << endl;
        auto feedback = initPipes(argVector, toWait);
        if (getpid() != mainPID) {
            if (feedback[0].compare("-5") == 0) {
                return -5;
            } else {
                argVector.clear();
                argVector = feedback;
            }
            string line;
            //we need to update the flags since 
            for(auto arg:argVector)
                line.append(arg);
            flagger(line, conf);    


            if (conf[0] == 1 || conf[1] == 1 || conf[2] == 1) {
                int redir = InitialzeRedir(conf, argVector);
                if (redir != 0)
                    return redir;
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
                cerr << "its was: " << args[0] << "\n\n"
                     << endl;
                for (int j = 0; j < i; j++)
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
                args[i] = strdup(arg.c_str());
                i++;
            }
            args[i] = NULL;
            int code = execvp(args[0], args);
            if (code == -1) {
                perror("Execution");
                cout << "np its was: " << args[0] << endl;
                for (int j = 0; j < i; j++)
                    free(args[j]);
                return -5;
            }
        } else {
            int status;
            waitpid(pid, &status, waitOpt);
            if (waitOpt == 0)
                return statusChecker(status, pid, argVector[0]);
        }
    }

    //main should do this
    int status;
    sleep(3);
    // for (auto p : toWait) {
    //     cout<<"Waiting for:"<<p<<endl;
    //     waitpid(p, &status, waitOpt);
    // }
    return 0;
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
        string str = to_string(WEXITSTATUS(status));
        better_set("EXITCODE", str);
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
