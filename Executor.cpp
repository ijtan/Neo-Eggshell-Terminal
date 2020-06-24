#include "Executor.h"

#include "StoppedHelper.h"
string waitingProcName;
pid_t waitingProcPid = -1;
vector<proc> StoppedProcs;
using namespace std;

int Executor(vector<string> &argVector, string &line, vector<int> conf) {
    int failed = 0;

    int waitOpt = 0;
    if (conf[4] == 1)
        waitOpt = WNOHANG;
    else
        waitOpt = WUNTRACED;

    if ((internalChecker(argVector[0]) == 1) && (conf[0] == 1 || conf[1] == 1 || conf[2] == 1)) {  //if command is ParentOnly but there are redirects -> abort (pipe checks itself)
        cout << "One or more internal commands entered are non rediractable. Aborting..." << endl;
        return 0;
    } else if ((internalChecker(argVector[0]) == 1) && !(conf[0] == 1 || conf[1] == 1 || conf[2] == 1 || conf[3] == 1)) {  //could simply check for (conf[1..3]==0) same thing.
        internalHandlerNoCHild(argVector[0], argVector);
        return 0;
    }

    pid_t mainPID = getpid();
    vector<needWaits> feedback;
    needWaits internalFeedBack;

    if (conf[3] == 1) {
        //Here we will intialize piping
        feedback = initPipes(argVector);
        if (feedback[0].returnCode != 0)
            return 0;
        if (getpid() != mainPID) {
            argVector.clear();
            argVector = feedback[0].newArgV;

            // re creation of line used for checking rediractions flags
            line.clear();
            for (auto arg : argVector) {
                line.append(arg);
                line.append(" ");
            }
            flagger(line, conf);
            conf[3] = 1;
        }
    }

    //
    if (getpid() == mainPID && conf[3] == 0) {  //redir no pipes
        // main should be here if no piping ->  since yet we have no forks and no
        // internal commands. we fork here so only the child is redirected / executed and the main class, adds the pid to the list of pids to wait for, then simply moves on.
        int pid = fork();
        flagger(line, conf);
        if (pid == -1)
            perror("fork");
        if (getpid() == mainPID) {  //main should to this
            internalFeedBack.newArgV = argVector;
            internalFeedBack.PID = pid;
            feedback.push_back(internalFeedBack);
        }
    }

    // flagging of new pipe-cut line (invidiual exec

    if ((getpid() != mainPID) && (conf[0] == 1 || conf[1] == 1 || conf[2] == 1)) {
        if (argVector.size() < 3) {
            cerr << "Not enough arguments provided for redirection. Aborting..." << endl;
            _exit(EXIT_FAILURE);
        }
        if (InitialzeRedir(conf, argVector) != 0) {
            cerr << "File redirection Failed!" << endl;
            _exit(EXIT_FAILURE);
        }
    }

    if (getpid() != mainPID) {
        if (internalHandler(argVector[0], argVector) == 0)
            // checks if the internalHandler matched; meaning that an internal
            // command was run and we do not need further execution
            _exit(EXIT_SUCCESS);

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
            cerr << "Error encountered when trying to execute '" << args[0] << "'." << endl;
            for (int j = 0; j < i; j++)
                free(args[j]);
            _exit(EXIT_FAILURE);
        }
    } else {
        int status;
        int i = 0;
        for (auto currProc : feedback) {
            waitingProcName = currProc.newArgV[0];
            waitingProcPid = currProc.PID;
            waitpid(currProc.PID, &status, waitOpt);
            if (waitOpt != WNOHANG)
                statusChecker(status, currProc.PID, currProc.newArgV[0]);
            i++;
        }
        waitingProcPid = -1;
    }

    return 0;
}

int statusChecker(int status, pid_t pid, string name) {
    if (WIFSIGNALED(status)) {
        cout << "Process Has Ended with a Signal!\nSignal Code " << WTERMSIG(status) << ":"
             << sys_siglist[WTERMSIG(status)] << endl;

        return -1;
    }
    if (WIFSTOPPED(status)) {
        cout << "Process Has Been Stopped!" << endl;
        addProc(getWaitingProc().name, getWaitingProc().pid);  //here we might have a provlem if the process terminates and the Waitingproc name changes, although not dangerous, since we would simply have amismatch in the name, i think this could be done better, maybe locking it ?
        return 0;
    }

    string str = to_string(WEXITSTATUS(status));
    setenv("EXITCODE", str.c_str(), 1);
    return 0;

    return 0;
}

process getFirstProc() {
    if (StoppedProcs.empty())
        return {"empty", -1};
    return {StoppedProcs[0].name, StoppedProcs[0].pid};
}
void incrementProcs() {
    if (!StoppedProcs.empty())
        StoppedProcs.erase(StoppedProcs.begin());
}
void addProc(string name, pid_t pid) {
    proc p;
    p.name = name;
    p.pid = pid;
    StoppedProcs.push_back(p);
}
process getWaitingProc() {
    return {waitingProcName, waitingProcPid};
}
vector<process> getProcVec() {
    vector<process> ret;
    for (auto procc : StoppedProcs)
        ret.push_back({procc.name, procc.pid});
    return ret;
}