#include "external.h"


using namespace std;
vector<proc> StpProcs;

int runExt(vector<string> &argVector, int *conf) {
    // set last token to NULL
    char *args[argVector.size()+1];
    int i = 0;
    for (const auto &str:argVector) {
        args[i] = const_cast<char *>(str.c_str());
        i++;
    }
    args[i] = (NULL);
    int maxArgSize = i;

    //0: >>
    //1: >
    //2: <
    //3: |
    //4: & -> background run
    int waitOpt = 0;

    if (conf[4] == 1)
        waitOpt = WNOHANG;

    pid_t pid = fork();
    if (pid == 0) {

        if (conf[2] == 1) {
            int count = 0;
            int j = 0;
            int argno = 0;

            for (auto arg:args) {
                if (arg != NULL && strlen(arg) == 1 && strncmp(arg, "<", 1) == 0) {
                    count++;
                    j = argno;
                }
                argno++;
            }

            if (count > 1) {
                cout << "Multiple input specifiers found! Aborting..." << endl;
                return -5;
            }

            if (j != 1) {
                cout << "Input specifier position invalid! Aborting..."<< endl;
                return -5;
            }
            if (argno < 3) {
                cout << "Command to redirect not specified" << endl;
                return -5;
            }

            //cutting the first 2 characters
            input(args[0]);
            if (argno == 3) {
                args[0] = args[2];
                args[1] = NULL;
            } else {
                int k = 0;
                cout<<"moving"<<endl;
                while (args[k + 2] != NULL && k < argno) {
                    cout<<"moved: "<<args[k+2]<<"--into--"<<args[k]<<endl;
                    strcpy(args[k],args[k+2]);
                    //args[k] = args[k + 2];
                    k++;
                }
                args[k-2]=NULL;
            }

        }


        if (conf[0] == 1 || conf[1] == 1) {
            char cmp[5] = ">>";
            int len = 2;
            if (conf[1] == 1) {
                strcpy(cmp, ">");
                len = 1;
            }


            int count = 0;
            int j = 0;
            int argno = 0;
            for (auto arg:args) {
                if (arg != NULL && strlen(arg) == len && strncmp(arg, cmp, len) == 0) {
                    count++;
                    j = argno;
                }
                cout<<"argno:"<<argno<<endl;
                argno++;
            }
            cout<<"argnof:"<<argno<<endl;
            if (count > 1) {
                cout << "Multiple input specifiers found! Aborting..." << endl;
                return -5;
            }
            if (j != argno - 3) {
                cout << "Output specifier position invalid! Aborting..." << endl;
                cout << "ExpectedL"<<argno-3<<" got "<<j << endl;
                cout << "out "<<args[argno-3]<<endl;
                return -5;
            }
            cout<<"filename"<<args[argno-2]<<endl;
            if (conf[1] == 1)
                truncOut(args[argno-2]);
            else
                append(args[argno-2]);
            args[argno-3] = NULL;
        }


        if (conf[3] == 1) {

        }


        signal(SIGINT, sigHandler);
        int code = execvp(args[0], args);
        if (code == -1) {
            perror("Execution");
        }
        return -5;
    }

    int status;
    waitpid(pid, &status, waitOpt
    );
    if (waitOpt == 0)
        return
                statusChecker(status, pid, argVector[0]
                );
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


//int runExtRedir(vector<string> argVector, char *buf, std::size_t size) {
//    // set last token to NULL
//    char *args[255];
//    int i = 0;
//    for (const auto &str:argVector) {
//        args[i] = const_cast<char *>(str.c_str());
//        i++;
//    }
//    args[i] = (NULL);
//
//
//    int fd[2];
//    if (pipe(fd) == -1) {
//        perror("pipe");
//        return -1;
//    };
//    pid_t pid = fork();
//    if (pid < 0) {
//        perror("fork");
//        return -1;
//    };
//
//
//    if (pid == 0) {
//        close(fd[0]);
//        dup2(fd[1], STDOUT_FILENO);
//        close(fd[1]);
//        int code = execvp(args[0], args);
//        if (code == -1) {
//            perror("Execution");
//        }
//    } else {
//        int status;
//        close(fd[1]);
//        int count = read(fd[0], buf, size - 1);
//        if (count == -1) {
//            perror("read");
//            return -1;
//        }
//
//        buf[count] = '\0';
//        cout << "INTERNAL REDIR OUT: '" << buf << "'; Size: " << size << endl;
//        if (wait(&status) == -1) {
//            perror("wait");
//            return -1;
//        }
//        if (WIFEXITED(status)) {
//            char env[sizeof(WEXITSTATUS(status)) + 10];
//            sprintf(env, "EXITCODE=%d", WEXITSTATUS(status));
//            putenv(env);
//        } else
//            return 0;
//        if (WIFSIGNALED(status)) {
//            cout << "exited with signal: " << WTERMSIG(status) << endl;
//            return -1;
//        }
//    }
//    return 0;
//}


vector<proc2> getStpProcs() {
    vector<proc2> procs2;
    for (auto prc:StpProcs) {
        proc2 tmp{prc.pid, prc.name};
        procs2.push_back(tmp);
    }
    return procs2;
}