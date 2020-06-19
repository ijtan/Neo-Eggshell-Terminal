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
    cout << "Goodbye!" << endl;

    free(line);
    freeVars();
    args.clear();
}

vector<string> env;

void lineReadInit() {

    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);
    initVars(env);

    string shell(getenv("SHELL"));
    shell.append(HistoryFileName);
    linenoiseHistoryLoad(shell.c_str());
    pid_t callerID = getppid();
    //start linenoise loop

    while ((line = linenoise(getenv("PROMPT"))) != NULL) {
        linenoiseHistoryAdd(line);
        linenoiseHistorySave(shell.c_str());

        // prepare for tokenization
        char copy[sizeof(line)];
        strcpy(copy, line);
        if (tokenize(line, copy, args) == -1) {
            continue;
        };

        //prepare to start parsin which runs externals commands
        char copy2[sizeof(line)];
        strcpy(copy2, line);
        int pl = (parseLine(copy2, args));
         //function's job done,command has been hadnled; prepare for next command
        args.clear();

        if (getppid() != callerID) {
            cout << "Unkilled fork detected, aborting child: (" << getppid() << "!=" << callerID << ')' << endl;
            _exit(EXIT_FAILURE);
        }
        
        if (getenv("PROMPT") == NULL or getenv("SHELL") == NULL)
            initVars(env);

        linenoiseFree(line);
    }
}

using namespace std;

int main(int argc, char *argv[]) {
    atexit(exitRoutine);
    auto sigoldINT = sigHandInstaller(SIGINT);
    auto sigoldTSTP = sigHandInstaller(SIGTSTP);

    cout << "Welcome to EggShell!" << endl;
    
    lineReadInit();

    return EXIT_SUCCESS;
}
