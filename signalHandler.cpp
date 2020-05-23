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

void sigHandler(int signum) {
    if (signum == SIGINT)
        exit(0);
    char print[255];
    snprintf(print, sizeof(print), "Caught Signal %s\n", sys_siglist[signum]);
    write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));

    if (signum == 18) {
        extern vector<proc> StpProcs;

        kill(StpProcs[0].pid, 18);
        StpProcs.erase(StpProcs.begin());
    }
    if (kill(getpid(), SIGINT) == 0) {
        snprintf(print, sizeof(print), "Killed process successfully!");
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));
    } else {
        snprintf(print, sizeof(print), "Process could not be killed!");
        write(STDOUT_FILENO, print, strnlen(print, sizeof(print)));

    }
}
