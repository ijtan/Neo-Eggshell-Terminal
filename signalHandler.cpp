#include "signalHandler.h"
//this class will be dedicated for trapping SIG faults which might occur throughout the runtime
using namespace std;

void neoSigHand(int signum) {
    char print[255];
    snprintf(print, sizeof(print), "Handler Caught Signal: [%s]\n", sys_siglist[signum]);
    write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));

    if (signum == SIGTSTP) {
        memset(&print[0], 0, sizeof(print));
        snprintf(print, sizeof(print), "Stopping Process: (%s:%d)\n", getCurrProc().name.c_str(), getCurrProc().pid);
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
        kill(getCurrProc().pid, SIGTSTP);
        pushProc(proc2{getCurrProc().pid, getCurrProc().name});
    } else if (signum == SIGINT) {
        snprintf(print, sizeof(print), "Interrupting Process: (%s:%d)\n", getCurrProc().name.c_str(),
                 getCurrProc().pid);
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
        kill(getCurrProc().pid, SIGINT);
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
    auto procs = getStpProcs();
    if (procs.size() == 0) {
        cout << "No Processed to Resume!" << endl;
        return;
    }
    int result = kill(procs[0].pid, SIGCONT);
    if (result == 0) {
        cout << "Process Resumed! (" << procs[0].name << ':' << procs[0].pid << ')' << endl;
        nextStpProcs();
    }
}
