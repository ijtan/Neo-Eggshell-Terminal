#include "external.h"

using namespace std;
vector<proc> StpProcs;

int Executer(vector<string>& argVector, vector<int> conf) {
    int waitOpt = 0;
    if (conf[4] == 1)
        waitOpt = WNOHANG;

    if((internalChecker(argVector[0])==1) && (conf[0] == 1 || conf[1] == 1 || conf[2] == 1)){ //if command is ParentOnly but there are redirects -> abort (pipe checks itself)
        cout << "One or more internal commands entered are non redircatbale. Aborting..." << endl;
        return 0;
    }else if((internalChecker(argVector[0])==1) && !(conf[0] == 1 || conf[1] == 1 || conf[2] == 1 || conf[3] == 1)){ //could simply check for (conf[1..3]==0) same thing.
        internalHandlerNoCHild(argVector[0], argVector);
        return 0;
    }

    pid_t mainPID = getpid();
    vector<PostPipes> feedback;
    PostPipes internalFeedBack;
        if (conf[3] == 1) {
        feedback = initPipes(argVector);
        if (feedback[0].returnCode == 99)
            return 0;
        if (getpid() != mainPID) {
            argVector.clear();
            argVector = feedback[0].newArgV;
            string line;
            vector<int> conf(5);
            for (auto arg : argVector)
                line.append(arg);

            // re creation of line used for checking rediractions flags
            flagger(line, conf);  // flagging of new pipe-cut line (invidiual exec
                                  // rather than all pipes)
        }
        }

        if (conf[3] == 0) {  //redir no pipes
        // main should be here if no piping ->  since yet we have no forks and no
        // internal commands we fork so only the child is redirected / executed and the main class simply waits
        int pid = fork();
        if (pid == -1)
            perror("redirection fork");
        if (getpid() == mainPID)  //would mean it sthe pare
            internalFeedBack.newArgV = argVector;
            internalFeedBack.PID = pid;
            feedback.push_back(internalFeedBack);
        }

        if ((getpid() != mainPID) && (conf[0] == 1 || conf[1] == 1 || conf[2] == 1)) {
        // making sure unset and cd are not used in redirects! this would cuase
        // the action to be done child side only!
        if (InitialzeRedir(conf, argVector) != 0)
            _exit(EXIT_FAILURE);
        }
        
        
  if (getpid() != mainPID) {
        if (internalHandler(argVector[0], argVector) == 0)
            // checks if the internalHandler matched; meaning that an internal
            // command was run and we do not need further execution
            _exit(EXIT_SUCCESS);

        char* args[argVector.size()];
        int i = 0;
        for (auto arg : argVector) {
            args[i] = strdup(arg.c_str());
            i++;
        }
        args[i] = NULL;

        int code = execvp(args[0], args);
        if (code == -1) {
            perror("Execution");
            cout << "Command was: " << args[0] << endl;
            for (int j = 0; j < i; j++)
                free(args[j]);
            _exit(EXIT_FAILURE);
        }
  }else{
        int status;
        int i = 0;
        for (auto currProc : feedback) {
            waitpid(currProc.PID, &status, waitOpt);
            statusChecker(status, currProc.PID, currProc.newArgV[0]);
            i++;
        }
}
return 0;
}

int statusChecker(int status, pid_t pid, string name) {
    if (WIFSIGNALED(status)) {
        cout << "Signal Code " << WTERMSIG(status) << ":"
             << sys_siglist[WTERMSIG(status)] << endl;
        if (WTERMSIG(status) == SIGTSTP)
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
