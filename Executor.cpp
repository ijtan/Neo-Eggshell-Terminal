#include "Executor.h"

#include "StoppedHelper.h"
//these will be used to hole the details of process being waited for
string waitingProcName; 
pid_t waitingProcPid = -1;
//this holds all processes which were stopped using sigtstp, used for the 'bg' and listproc commands
vector<proc> StoppedProcs;

using namespace std;

int Executor(vector<string> &argVector, string &line, vector<int> conf) {
    int waitOpt = 0; //this is used in case of ampersand command
    if (conf[4] == 1)
        waitOpt = WNOHANG; //means dont hang waiitng for process
    else
        waitOpt = WUNTRACED; //wait for process

    if ((internalChecker(argVector[0]) == 1) && (conf[0] == 1 || conf[1] == 1 || conf[2] == 1)) {  //if command is ParentOnly but there are redirects -> abort (pipe checks itself)
        cout << "One or more internal commands entered are non rediractable. Aborting..." << endl;
        return 0;
    } else if ((internalChecker(argVector[0]) == 1) && !(conf[0] == 1 || conf[1] == 1 || conf[2] == 1 || conf[3] == 1)) {  //could simply check for (conf[1..3]==0) same thing.
        internalHandlerNoCHild(argVector[0], argVector);
        return 0;
    }

    pid_t mainPID = getpid(); //store the pid of main to be able to identify later on

    //this will be used to hold details about all processes forked, and thus all processes which need waiting
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
        
        //convert arg vector into array of strings
        char *args[argVector.size()];
        int i = 0;
        for (auto arg : argVector) {
            args[i] = strdup(arg.c_str());
            i++;
        }
        args[i] = NULL; //make sure its null terminated

        int code = execvp(args[0], args); //actual exec, and store return
        if (code == -1) {
            //report error to user
            perror("Execution");
            cerr << "Error encountered when trying to execute '" << args[0] << "'." << endl;
            for (int j = 0; j < i; j++) //free args which were strduped, since the exec has failed
                free(args[j]);
            _exit(EXIT_FAILURE); //exit the process which is the failed exec
        }
    } else {
        int status;
        int i = 0;
        for (auto currProc : feedback) { //for all process which need to be waited for
        //update the waitingProc details which may be used in siganling
            waitingProcName = currProc.newArgV[0];
            waitingProcPid = currProc.PID;
            //start wait
            waitpid(currProc.PID, &status, waitOpt);
            if (waitOpt != WNOHANG)
                statusChecker(status, currProc.PID, currProc.newArgV[0]); //report any siganls and update the EXITCODE
            i++;
        }
        waitingProcPid = -1; //set pid to -1 so to show we are not waiitng for anything
    }

    return 0;
}

int statusChecker(int status, pid_t pid, string name) {
    if (WIFSIGNALED(status)) { //check if process ended with a signal
        cout << "Process Has Ended with a Signal!\nSignal Code " << WTERMSIG(status) << ":"
             << sys_siglist[WTERMSIG(status)] << endl;

        return -1;
    }
    if (WIFSTOPPED(status)) {  //check if process was stopped
        cout << "Process Has Been Stopped!" << endl;
        //add process to stoppedProcs
        addProc(getWaitingProc().name, getWaitingProc().pid);  
        //^here we might have a provlem if the process terminates and the Waitingproc name changes, although not dangerous, since we would simply have a mismatch in the name, i think this could be done better, maybe locking it ?
        return 0;
    }

    string str = to_string(WEXITSTATUS(status));  //update exitcode
    setenv("EXITCODE", str.c_str(), 1);
    return 0;
}

process getFirstProc() {
    //return first process in the stopped prcesses vector
    if (StoppedProcs.empty())
        return {"empty", -1};
    return {StoppedProcs[0].name, StoppedProcs[0].pid};
}
void incrementProcs() {
    //remove first process in the stopped processes vectore
    if (!StoppedProcs.empty())
        StoppedProcs.erase(StoppedProcs.begin());
}
void addProc(string name, pid_t pid) {
    //thiss is used to add a process to the stopped processes vector
    proc p;
    p.name = name;
    p.pid = pid;
    StoppedProcs.push_back(p);
}
process getWaitingProc() {//this returns the process currently being waited for
    return {waitingProcName, waitingProcPid};
}
vector<process> getProcVec() { //this returns the whole stopped process vector, it needs to conver from type proc to type process
    vector<process> ret;
    for (auto procc : StoppedProcs)
        ret.push_back({procc.name, procc.pid});
    return ret;
}