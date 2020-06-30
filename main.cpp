#include <cstdlib>
#include <cstring>
#include <iostream>

#include "ext/linenoise.h"
#include "parser.h"
#include "signalHandler.h"

#define MAX_HISTORY 25
#define HistoryFileName "LineHistory.txt"

char *line;
vector<string> args;

void exitRoutine() {
    //kill all child process which were stopped -> avoid orphaned stopped children
    for(auto proc:getProcVec())
        kill(proc.pid,SIGKILL);
    //free the last line, as we never free it otherwise
    free(line);
    //celar arg vector for good measure
    args.clear();
    //Send a nice message to the user
    cout << "Goodbye!" << endl;
}

vector<string> env;

void lineReadInit() {
    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY); //set max history
    initVars(); //init the main variables of the system 

    string shell(getenv("SHELL")); 
    //use the shell variable as the place to store the linenoiseHistory file. 
    //If this was not the case and we would store it with just a filename instead of a path, 
    //the file would be stored wherever the CWD is, and thus endup with a bunch of history files everywhere
    shell.append(HistoryFileName);
    linenoiseHistoryLoad(shell.c_str());
    //store pid of whatever called the main (usually init); used for error checking
    pid_t callerID = getppid();
    //start linenoise loop
    while ((line = linenoise(getenv("PROMPT"))) != NULL) { //display prompt and take input
    //save the history to file
        linenoiseHistoryAdd(line);
        linenoiseHistorySave(shell.c_str());

        // prepare for tokenization
        char copy[sizeof(line)];
        strcpy(copy, line);
        if (tokenize(line, copy, args) == -1) {
            continue;
        }
        //prepare to start parsin which runs externals commands
        char copy2[sizeof(line)];
        strcpy(copy2, line);
        int pl = (parseLine(copy2, args));
        //function's job done,command has been hadnled; prepare for next command
        args.clear();
        //ensure that only the main process called by init is here, and not ones called by the main process (not escaped children). 
        if (getppid() != callerID) {
            cout << "Unkilled fork detected, aborting child: (" << getppid() << "!=" << callerID << ')' << endl;
            _exit(EXIT_FAILURE);
        }
        //if prompt or shell are null, we have to make sure that these are reset, else we would get a segmentation fault
        if (getenv("PROMPT") == NULL or getenv("SHELL") == NULL){
            cerr<<"PROMPT or SHELL variabels were nulled, resetting to prevent major errors"<<endl;
            initVars();}
        //free the line which was allocated by linenoise
        linenoiseFree(line);
    }
}

using namespace std;

int main(int argc, char *argv[]) {
    //define what the program shuould do yupon exiting
    atexit(exitRoutine);
    //install handlers for SIGINT and SIGTSTP
    sigHandInstaller(SIGTSTP); 
    sigHandInstaller(SIGINT);
    //display a welcome message to the user (with color)
    cout << "\033[1;33m"
         << "Welcome to EggShell!"
         << "\033[0m" << endl;
    //start the method which has main loop
    lineReadInit();

    return EXIT_SUCCESS;
}
