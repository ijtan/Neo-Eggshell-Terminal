#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ext/linenoise.h"
#include "parser.h"
#include "signalHandler.h"

#define MAX_HISTORY 25
//extern char**environ;

char *line;
vector<string> args;
void exitRoutine() {
    cout<<"Goodbye!"<<endl;
    free(line);
    freeVars();
    args.clear();
}

void lineReadInit() {


    atexit(exitRoutine);

    vector<string> env;
    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);
    initVars(env);
    //start linenoise loop
    while ((line = linenoise(getenv("PROMPT"))) != NULL) {

        linenoiseHistoryAdd(line);
        char copy[sizeof(line)];
        strcpy(copy, line);
        if (tokenize(line, copy, args) == -1) { continue; };
        //string copy(line);


        //call function which runs externals commands
        char copy2[sizeof(line)];
        strcpy(copy2, line);
        int pl = (parseLine(copy2, args));
        linenoiseFree(line);
        args.clear();
        if (pl == -5)
            _exit(-5);
        if (getenv("PROMPT") == NULL or getenv("SHELL") == NULL) {
            initVars(env);
        }
    }
}


using namespace std;


int main(int argc, char *argv[]) {
    signal(SIGINT, sigHandler);
    cout << "Welcome to EggShell!" << endl;
    //init vars
    lineReadInit();

    // from documentation:
//    int linenoiseHistorySave(const char *filename);
//    int linenoiseHistoryLoad(const char *filename);

    return 0;
}
