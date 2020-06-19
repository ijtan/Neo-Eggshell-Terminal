#include "signalHandler.h"


//this class will be dedicated for trapping SIG faults which might occur throughout the runtime
using namespace std;

void neoSigHand(int signum) {
    char print[255];
    snprintf(print, sizeof(print), "Handler Caught Signal: [%s]\n", sys_siglist[signum]);
    write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));

    if (signum == SIGTSTP) {
        memset(&print[0], 0, sizeof(print));
        snprintf(print, sizeof(print), "Stopping Process: (%s:%d)\n", getWaitingProc().name.c_str(), getWaitingProc().pid);
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
        kill(getWaitingProc().pid, SIGTSTP);
        addProc(getWaitingProc().name, getWaitingProc().pid);
    } else if (signum == SIGINT) {
        snprintf(print, sizeof(print), "Interrupting Process: (%s:%d)\n", getWaitingProc().name.c_str(),
                 getWaitingProc().pid);
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
        kill(getWaitingProc().pid, SIGINT);
    }
}

sig_t sigHandInstaller(int signum) {
    sig_t prevHand = signal(signum, neoSigHand);
    if (prevHand == SIG_ERR)
        cout << "ERROR Could not install Handler! (" << sys_siglist[signum] << ")" << endl;
    // else
    //     cout << "Installed Handler! (" << sys_siglist[signum] << ")" << endl;
    return prevHand;
}

void resumeStopped() {
    if(getFirstProc().pid == -1){
        cout << "No Processes to Resume!" << endl;
        return;
    }
    int result = kill(getFirstProc().pid, SIGCONT);
    if (result == 0) {
        cout << "Process Resumed! (" << getFirstProc().name << ':' << getFirstProc().pid << ')' << endl;
        incrementProcs();
    }
}
