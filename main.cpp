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
    args.clear();
}

vector<string> env;

void lineReadInit() {
    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);
    initVars();

    string shell(getenv("SHELL"));
    shell.append(HistoryFileName);
    linenoiseHistoryLoad(shell.c_str());
    pid_t callerID = getppid();
    //start linenoise loop

    while ((line = linenoise(("\033[1;36m"+ string(getenv("PROMPT"))+"\033[0m").c_str())) != NULL) {
       
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
            initVars();

        linenoiseFree(line);
    }
}

using namespace std;

int main(int argc, char *argv[]) {
    atexit(exitRoutine);

    sigHandInstaller(SIGTSTP);
    sigHandInstaller(SIGINT);
    cout << "\033[1;33m" << "Welcome to EggShell!" << "\033[0m"<<endl;

    lineReadInit();

    return EXIT_SUCCESS;
}
