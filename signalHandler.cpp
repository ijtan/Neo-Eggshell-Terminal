#include "signalHandler.h"

//this class will be dedicated for trapping SIG faults which might occur throughout the runtime
using namespace std;

void sigIntHandle(int signum) { //this handles sigints
    pid_t pidtokill = getWaitingProc().pid; //get the process currently being waited for 
    kill(pidtokill, SIGINT); //kill with sigint
}

void sigTSTPHandle(int signum) { //handles sigtstp
    const char msg[] = "Handler Caught SIGTSTP\n"; //art the user that a signal is being handled
    write(STDOUT_FILENO, msg, strnlen(msg, sizeof(msg)));

    pid_t pidToKill = getWaitingProc().pid; //set pif to kill by getting the process currently being waited for
    kill(pidToKill, SIGTSTP);
    //addProc(getWaitingProc().name, pidToKill); //here we might have a provlem if the process terminates and the Waitingproc name changes, although not dangerous, since we would simply have amismatch in the name, i think this could be done better, maybe locking it ?
}

void sigHandInstaller(int signum) { //this installs the handlers required
    sig_t prevHand = SIG_ERR;
    //matches the signup to the required one and calls signal with it, thus setting out handler instead of the default one 
    if (signum == SIGTSTP)
        prevHand = signal(signum, sigTSTPHandle);
    if (signum == SIGINT)
        prevHand = signal(signum, sigIntHandle);
    if (prevHand == SIG_ERR)  //async doesnt matter here since the installation happens on prgram start and before anything else
        cout << "ERROR Could not install Handler! (" << sys_siglist[signum] << ")" << endl;
}

void resumeStopped() { //this funciton is used for the 'bg' command, this uses kill() to send sigcont to the first element in the stopped vector, then incrmenets it. 
    if (getFirstProc().pid == -1) { //enusre that it is not empty 
        cout << "No Processes to Resume!" << endl; //alrt user accordingly
        return;
    }
    int result = kill(getFirstProc().pid, SIGCONT); //kill
    if (result == 0) {
        cout << "Process Resumed! (" << getFirstProc().name << ':' << getFirstProc().pid << ')' << endl; //if result is 0, alrt success
        incrementProcs(); //increment the vector
    }
}
