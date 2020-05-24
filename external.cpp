#include "external.h"


using namespace std;
vector<proc> StpProcs;

int runExt(vector<string> &argVector, int *conf) {
    // set last token to NULL

    char *args[argVector.size() + 1];
    int i = 0;
    for (const auto &str:argVector) {
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

    if (conf[3] == 1) {
        cout << "init" << endl;

        //start piping
        int pipeCount = 0;
        char *args2[16][1024];
        int pos = 0;
        for (int i = 0; args[i] != NULL && i < 16; i++) {
            if (strncmp(args[pos], "|", 1) == 0) {
                pipeCount++;
                cout<<"pipe foudn"<<endl;
                args2[pipeCount][pos] = NULL;
                pos=0;
                continue;
            }
            args2[pipeCount][pos] = args[pos];
            cout<<"new args2: "<<args2[pipeCount][pos]<<endl;
            pos++;
        }
        //REFERENCE - CPS1012 - Redirection and Pipes (Sys Prog) Part 2 by Keith Bugeja

        cout << "pipes init" << endl;

        int fd[pipeCount * 2],
                *currFD = fd,
                *prevFD = NULL;

        cout<<"pipe count is:"<<pipeCount<<endl;
        pipeCount=0;
        for (int part = 0; part < pipeCount + 1; part++) {
            prevFD = currFD - 2;

            if (part < pipeCount)
                pipe(currFD);


            cout << "forking" << endl;
            pid_t PipepPid = fork();
            if (PipepPid == -1) {
                perror("Pipe fork");
                return -5;
            } else if (PipepPid == 0) {

                cout << "got forked: "<<getpid()<<" by: "<<getppid() << endl;


                cout << "starting copying" << endl;
                for (int cnt = 0; args2[cnt - 1] != NULL && cnt > pipeCount + 1; cnt++) {
                    cout << "copying" << endl;
                    cout << args[cnt] << endl;
                    cout << "=    " << endl;
                    cout << args2[part][cnt] << endl;

                    strcpy(args[cnt], args2[part][cnt]);
                }
                cout << "done copying" << endl;


                if (part < pipeCount) {
                    close(currFD[0]); // not needed since nothing will be passing to it;
                    dup2(currFD[1], STDOUT_FILENO);
                    close(currFD[1]); // not needed since is redirected to stdout
                }


                if (part > 0) {
                    close(prevFD[1]);
                    dup2(prevFD[0], STDIN_FILENO);
                    close(prevFD[0]);
                }
                currFD += 2;

            }
        }



        ////always return -5 since all executions follow from this one
        // return -5;
        cout << "pipes done" << endl;
    }

    if (pid == 0) {

        if (conf[2] == 1) {
            int count = 0;
            int j = 0;
            int argno = 0;

            for (
                auto arg
                    :args) {
                if (arg != NULL &&
                    strlen(arg)
                    == 1 &&
                    strncmp(arg,
                            "<", 1) == 0) {
                    count++;
                    j = argno;
                }
                argno++;
            }

            if (count > 1) {
                cout << "Multiple input specifiers found! Aborting..." <<
                     endl;
                return -5;
            }

            if (j != 1) {
                cout << "Input specifier position invalid! Aborting..." <<
                     endl;
                return -5;
            }
            if (argno < 3) {
                cout << "Command to redirect not specified" <<
                     endl;
                return -5;
            }

//cutting the first 2 characters
//cout << "zrgno" <<argno<< endl;


            input(args[0]);
            if (argno == 4) {
                args[0] = args[2];
                args[1] = NULL;
            } else {
                int k = 0;
//cout << "moving" << endl;
                while (args[k + 2] != NULL && k < argno) {
//cout << "moved: " << args[k + 2] << "--into--" << args[k] << endl;
                    strcpy(args[k], args[k + 2]
                    );
//args[k] = args[k + 2];
                    k++;
                }
                args[k - 2] = NULL;
            }

        }


        if (conf[0] == 1 || conf[1] == 1) {
            char cmp[5] = ">>";
            int len = 2;
            if (conf[1] == 1) {
                strcpy(cmp,
                       ">");
                len = 1;
            }


            int count = 0;
            int j = 0;
            int argno = 0;
            for (
                auto arg
                    :args) {
                if (arg != NULL &&
                    strlen(arg)
                    ==
                    len && strncmp(arg, cmp, len)
                           == 0) {
                    count++;
                    j = argno;
                }
                argno++;
            }
            if (count > 1) {
                cout << "Multiple input specifiers found! Aborting..." <<
                     endl;
                return -5;
            }
            if (j != argno - 3) {
                cout << "Output specifier position invalid! Aborting..." <<
                     endl;
                return -5;
            }
            if (conf[1] == 1)
                truncOut(args[argno - 2]);
            else
                append(args[argno - 2]);
            args[argno - 3] = NULL;
        }


        string tmp(args[0]);
        vector<string> argsVec;
        for (
            auto a
                :args) {
            if (a == NULL)
                break;
            string tmp(a);
            argsVec.
                    push_back(tmp);
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
    }

    int status;
    waitpid(pid, &status, waitOpt);
    if (waitOpt == 0)
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
    for (auto prc:StpProcs) {
        proc2 tmp{prc.pid, prc.name};
        procs2.push_back(tmp);
    }
    return procs2;
}