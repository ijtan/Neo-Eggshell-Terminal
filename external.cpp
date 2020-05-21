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
            exit(-1);
        }
        exit(0);
    }
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
        cout << "exited status: " << WEXITSTATUS(status) << endl;
    if (WIFSIGNALED(status)) {
        cout << "exited with signal: " << WTERMSIG(status) << endl;
    }
    return 0;
}

void runExtRedir(vector<string> argVector, char *buf, std::size_t size) {
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
        return;
    };
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    };


    if (pid == 0) {
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[1]);
        int code = execvp(args[0],args);
        if(code == -1){
           perror("Execution");
            exit(-1);
         }
        exit(0);
    } else {
        int status;
        close(fd[1]);
        int count = read(fd[0],buf,size-1);
        if(count ==-1){
            perror("read");
        }

        buf[count] = '\0';
        cout << "INTERNAL REDIR OUT: '" << buf<<"'; Size: "<<size << endl;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            cout << "exited status: " << WEXITSTATUS(status) << endl;
        if (WIFSIGNALED(status)) {
            cout << "exited with signal: " << WTERMSIG(status) << endl;
        }
    }
}