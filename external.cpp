#include "external.h"

#include <utility>

using namespace std;
vector<proc> StpProcs;
int runExt(vector<string>& argVector, int *conf) {
    // set last token to NULL
    char *args[argVector.size() + 1];
    int i = 0;
    for (const auto &str:argVector) {
        args[i] = const_cast<char *>(str.c_str());
        i++;
    }
    args[i] = (NULL);

    if(conf[0]==1){

    }
    else if(conf[1]==1){

    }

    if(conf[2]==1){

    }
    if(conf[3]==1){

    }
    int waitOpt;
    if(conf[4]==1)
         waitOpt=WNOHANG;
    else
        waitOpt=0;

    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, sigHandler);
        int code = execvp(args[0], args);
        if (code == -1) {
            perror("Execution");
            _exit(-1);
        }
        _exit(0);
    }
    int status;
    waitpid(pid, &status, waitOpt);
    if (waitOpt == 0)
        return statusChecker(status,pid,argVector[0]);
    else
        return 0;
}


int statusChecker(int status, pid_t pid, string name){
    if (WIFSIGNALED(status)) {
        cout << "Signal Code " << WTERMSIG(status) <<":"<<sys_siglist[WTERMSIG(status)]<< endl;
        if(WTERMSIG(status) == 19)
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