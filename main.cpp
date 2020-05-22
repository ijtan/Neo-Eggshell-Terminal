#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ext/linenoise.h"
#include "parser.h"
#include "signalHandler.h"

#define MAX_HISTORY 25
#define MAX_ENV_VARS 1000
//extern char**environ;
void lineReadInit() {
    char *line;
    vector<string> args;
    vector<string> env;
    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);
    if(getenv("PROMPT")==NULL or getenv("SHELL")==NULL)
        initVars(env);
    string prompt = getenv("PROMPT");
    //start linenoise loop
    while ((line = linenoise(getenv("PROMPT"))) != NULL) {

        linenoiseHistoryAdd(line);
        char copy[sizeof(line)];
        strcpy(copy,line);
        if(tokenize(line, copy, args)==-1){ continue;};
        //string copy(line);


        //call function which runs externals commands
        if (args[0] != "exit") {
            parseLine(line, args);
            free(line);
            args.clear();
        } else {
            free(line);
            args.clear();
            freeVars();
            exit(EXIT_SUCCESS);

        }
        if(getenv("PROMPT")==NULL or getenv("SHELL")==NULL) {
            initVars(env);
            prompt = getenv("PROMPT");
        }
    }
}


using namespace std;


int main(int argc, char*argv[]) {
    signal(SIGINT, sigHandler);
    cout << "Welcome to EggShell!" << endl;
    //init vars
    lineReadInit();

    // from documentation:
//    int linenoiseHistorySave(const char *filename);
//    int linenoiseHistoryLoad(const char *filename);

    return 0;
}
