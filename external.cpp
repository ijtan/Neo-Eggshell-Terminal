#include "external.h"

using namespace std;

int runExt(vector<string> argVector) {
    // set last token to NULL
    char *args[argVector.size() + 1];
    int i = 0;
    for (const auto &str:argVector) {
        args[i] = const_cast<char *>(str.c_str());
        i++;
    }
    args[i] = (NULL);
    pid_t pid = fork();
    if (pid == 0) {
        int code = execvp(args[0], args);
        if (code == -1) {
            perror("Execution");
            _exit(-1);
        }
        _exit(0);
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) != 0) {
        char envStr[sizeof(WEXITSTATUS(status)) + 10];
        sprintf(envStr, "EXITCODE=%d", (WEXITSTATUS(status)));
        vector<string> env;
        env.emplace_back(envStr);
        set(env);
    } else
        return 0;
    if (WIFSIGNALED(status)) {
        cout << "exited with signal: " << WTERMSIG(status) << endl;
        return -1;
    }
    return 0;
}

int runExtRedir(vector<string> argVector, char *buf, std::size_t size) {
    // set last token to NULL
    char *args[255];
    int i = 0;
    for (const auto &str:argVector) {
        args[i] = const_cast<char *>(str.c_str());
        i++;
    }
    args[i] = (NULL);


    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        return -1;
    };
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    };


    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        int code = execvp(args[0], args);
        if (code == -1) {
            perror("Execution");
        }
    } else {
        int status;
        close(fd[1]);
        int count = read(fd[0], buf, size - 1);
        if (count == -1) {
            perror("read");
            return -1;
        }

        buf[count] = '\0';
        cout << "INTERNAL REDIR OUT: '" << buf << "'; Size: " << size << endl;
        if (wait(&status) == -1) {
            perror("wait");
            return -1;
        }
        if (WIFEXITED(status)) {
            char env[sizeof(WEXITSTATUS(status)) + 10];
            sprintf(env, "EXITCODE=%d", WEXITSTATUS(status));
            putenv(env);
        } else
            return 0;
        if (WIFSIGNALED(status)) {
            cout << "exited with signal: " << WTERMSIG(status) << endl;
            return -1;
        }
    }
    return 0;
}