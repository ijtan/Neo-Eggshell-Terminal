#include <iostream>
#include <cstdlib>
#include "linenoise.h"
#include "parser.h"
#include "rediraction.h"


using namespace std;
#define MAX_ARGS 100
#define MAX_HISTORY 20


int main() {
    cout << "Welcome to EggShell!" << endl;
    //init vars
    char *line,
            *token = NULL;
    vector<string> args;
    int tokenIndex;
    char buff[100];
    sprintf(buff, "PROMPT=%s@eggshell> ", getenv("USER"));
    putenv(buff);
    initVars();

    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);


    //start linenoise loop
    while ((line = linenoise(getenv("PROMPT"))) != NULL) {
        linenoiseHistoryAdd(line);
        string copy(line);
        if(copy.empty()){
            free(line);
            args.clear();
            continue;
        }
        token = strtok((char *) copy.c_str(), " ");
        for (tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1; tokenIndex++) {
            args.emplace_back(token);
            token = strtok(NULL, " ");
        }

        if (args[0] == "exit") {
            //TODO MAKE THIS INTERNAL AND WAY TO FREE ALL VARS
            free(line);
            break;
        }
        //call function which runs externals commands

        parseLine(line, args);
        free(line);
        args.clear();

    }

    // from documentation:
//    int linenoiseHistoryAdd(const char *line);
//    int linenoiseHistorySetMaxLen(int len);
//    int linenoiseHistorySave(const char *filename);
//    int linenoiseHistoryLoad(const char *filename);

    return 0;
}
