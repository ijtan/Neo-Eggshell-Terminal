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
    waitpid(pid, &status, 0);

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


    int fd[2];
    if (pipe(fd) == -1){perror("pipe");return NULL;};
    pid_t pid = fork();
    if (pid<0){perror("fork");return NULL;};


    if(pid == 0) {
        //siginterrupt(SIGALRM,10);
        dup2(fd[1],STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        char msg[] = "HELLO\n";
        write(STDOUT_FILENO,msg,sizeof(msg));
        //int code = execvp(args[0],args);
        //if(code == -1){
         //   perror("Execution");
        //    exit(-1);
       // }
        exit(0);
    }
    else{
        int status;
        close(fd[0]);
        char buf;
        string output;

        while(fgetc(fileno(fd[0]))){
            if(buf!=0)
                output.push_back(buf);
            else
                break;
        }
        close(fd[1]);
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            cout << "exited status: " << WEXITSTATUS(status) << endl;
        if (WIFSIGNALED(status)) {
            cout << "exited with signal: " << WTERMSIG(status) << endl;
        }
        return output;
    }
}