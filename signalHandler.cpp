#include "signalHandler.h"

//this class will be dedicated for trapping SIG faults which might occur throughout the runtime
using namespace std;

void sigIntHandle(int signum) {
    char msg[31];
    snprintf(msg, sizeof(msg), "Handler Caught Signal: [%s]\n", sys_siglist[SIGINT]);

    char print[255];
    snprintf(print, sizeof(print), "Interrupting Process: (%s:%d)\n", getWaitingProc().name.c_str(),
             getWaitingProc().pid);
    write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
    kill(getWaitingProc().pid, SIGINT);
}

void sigTSTPHandle(int signum) {
    char msg[31];
    snprintf(msg, sizeof(msg), "Handler Caught Signal: [%s]\n", sys_siglist[SIGTSTP]);
    write(STDOUT_FILENO, msg, strnlen(msg, sizeof(msg)));
    char print[255];
    snprintf(print, sizeof(print), "Stopping Process: (%s:%d)\n", getWaitingProc().name.c_str(), getWaitingProc().pid);
    write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
    pid_t pidToKill = getWaitingProc().pid;
    kill(pidToKill, SIGTSTP);
    addProc(getWaitingProc().name, pidToKill);  //here we might have a provlem if the process terminates and the Waitingproc name changes, although not dangerous, since we would simply have amismatch in the name, i think this could be done better, maybe locking it ?
}

void sigHandInstaller(int signum) {
    sig_t prevHand = SIG_ERR;
    if ((signum == SIGTSTP))
        prevHand = signal(signum, sigTSTPHandle);
    if ((signum == SIGINT))
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
