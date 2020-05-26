#include "signalHandler.h"
//this class will be dedicated for trapping SIG faults which might occur throughout the runtime

//ctrlz and ctrlc handler

//The ability to interrupt or suspend a process is very important; typically, shells reserve two keyboard
//shortcuts, CTRL + C and CTRL + Z , to signal running processes with an interruption or
//a suspension signal respectively.
//(a) Provide functionality for trapping interrupt (SIGINT) and suspend (SIGTSTP) signals; forward
//        the signal to the process currently running in the foreground.
//(b) Add the ability to run background processes to the program launcher; background processes
//are denoted by the use of the & operator at the end of a command string.
//(c) Add the internal command listproc to display the processes spawned through the shell,
//together with their process id (pid).
//(d) Add the internal command bg to resume a suspended process in the background.

using namespace std;

void neoSigHand(int signum)
{
    char print[255];
    snprintf(print, sizeof(print), "Handler Caught Signal: [%s]\n", sys_siglist[signum]);
    write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));

    if (signum == SIGTSTP)
    {   memset(&print[0], 0, sizeof(print));
        snprintf(print, sizeof(print), "Stopping Process: (%s:%d)\n",getCurrProc().name.c_str(), getCurrProc().pid);
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
        kill(getCurrProc().pid, SIGTSTP);
        pushProc(proc2{getCurrProc().pid, getCurrProc().name});
    }else if (signum == SIGINT)
    {
        snprintf(print, sizeof(print), "Interrupting Process: (%s:%d)\n",getCurrProc().name.c_str(), getCurrProc().pid);
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
        kill(getCurrProc().pid, SIGINT);
    }
}

sig_t sigHandInstaller(int signum)
{
    sig_t prevHand = signal(signum, neoSigHand);
    if (prevHand == SIG_ERR)
        cout << "ERROR Could not install Handler! (" << sys_siglist[signum] << ")" << endl;
    // else
    //     cout << "Installed Handler! (" << sys_siglist[signum] << ")" << endl;
    return prevHand;
}

void resumeStopped()
{
    auto procs = getStpProcs();
    if (procs.size() == 0)
    {
        cout << "No Processed to Resume!" << endl;
        return;
    }
    int result = kill(procs[0].pid, SIGCONT);
    if (result == 0)
    {
        cout << "Process Resumed! (" << procs[0].name << ':' <<procs[0].pid<<')' << endl;
        nextStpProcs();
    }
}
