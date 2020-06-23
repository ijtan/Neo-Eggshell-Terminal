#include "signalHandler.h"

//this class will be dedicated for trapping SIG faults which might occur throughout the runtime
using namespace std;

void sigIntHandle(int signum) {
    const char msg[] = "Handler Caught SIGINT\n";

    pid_t pidtokill = getWaitingProc().pid;
    kill(pidtokill, SIGINT);
}

void sigTSTPHandle(int signum) {
    const char msg[] = "Handler Caught SIGTSTP\n";
    write(STDOUT_FILENO, msg, strnlen(msg, sizeof(msg)));
    pid_t pidToKill = getWaitingProc().pid;
    kill(pidToKill, SIGTSTP);
    //addProc(getWaitingProc().name, pidToKill); //here we might have a provlem if the process terminates and the Waitingproc name changes, although not dangerous, since we would simply have amismatch in the name, i think this could be done better, maybe locking it ?
}

void sigHandInstaller(int signum) {
    sig_t prevHand = SIG_ERR;
    if (signum == SIGTSTP)
        prevHand = signal(signum, sigTSTPHandle);
    if (signum == SIGINT)
        prevHand = signal(signum, sigIntHandle);
    if (prevHand == SIG_ERR)  //async doesnt matter here since the installation happens on prgram start and before anything else
        cout << "ERROR Could not install Handler! (" << sys_siglist[signum] << ")" << endl;
}

void resumeStopped() {
    if (getFirstProc().pid == -1) {
        cout << "No Processes to Resume!" << endl;
        return;
    }
    int result = kill(getFirstProc().pid, SIGCONT);
    if (result == 0) {
        cout << "Process Resumed! (" << getFirstProc().name << ':' << getFirstProc().pid << ')' << endl;
        incrementProcs();
    }
}
