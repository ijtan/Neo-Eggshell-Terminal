#include <cstdlib>
#include <cstring>
#include <iostream>

#include "ext/linenoise.h"
#include "parser.h"
#include "signalHandler.h"

#define MAX_HISTORY 25
#define HistoryFileName "LineHistory"

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
    atexit(exitRoutine);

    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);
    initVars(env);

    string shell(getenv("SHELL"));
    shell.append("History.txt");
    linenoiseHistoryLoad(shell.c_str());
    pid_t callerID = getppid();
    //start linenoise loop

    while ((line = linenoise(getenv("PROMPT"))) != NULL) {
        linenoiseHistoryAdd(line);
        linenoiseHistorySave(shell.c_str());

        char copy[sizeof(line)];
        strcpy(copy, line);
        if (tokenize(line, copy, args) == -1) {
            continue;
        };
        //string copy(line);

        //call function which runs externals commands
        char copy2[sizeof(line)];
        strcpy(copy2, line);
        int pl = (parseLine(copy2, args));
        linenoiseFree(line);
        args.clear();

        
        if (pl == -5)
            _exit(-5);
        if (getppid() != callerID) {
            cout << "Unkilled fork detected, aborting child: (" << getppid() << "!=" << callerID << ')' << endl;
            _exit(EXIT_FAILURE);
        }
        if (getenv("PROMPT") == NULL or getenv("SHELL") == NULL)
            initVars(env);
    }
}

using namespace std;

int main(int argc, char *argv[]) {
    //signal(SIGINT, neoSigHand);
    auto sigoldINT = sigHandInstaller(SIGINT);
    auto sigoldTSTP = sigHandInstaller(SIGTSTP);
    cout << "Welcome to EggShell!" << endl;
    lineReadInit();

    return 0;
}
