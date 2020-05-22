#include <iostream>
#include <cstdlib>
#include "ext/linenoise.h"
#include "parser.h"
#include "signalHandler.h"



using namespace std;
#define MAX_ARGS 100
#define MAX_HISTORY 20
//extern char**environ;


int main(int argc, char*argv[]) {
    //signal(SIGINT, sigHandler);
    cout << "Welcome to EggShell!" << endl;
    //init vars

    char *line,
            *token = NULL;
    int tokenIndex;
    vector<string> args;
    vector<string> env;
    initVars(env);

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

        //call function which runs externals commands
        if(args[0]!="exit") {
            parseLine(line, args);
            free(line);
            args.clear();
        }
        else{
            free(line);
            args.clear();
            exit(0);
        }

    }

    // from documentation:
//    int linenoiseHistoryAdd(const char *line);
//    int linenoiseHistorySetMaxLen(int len);
//    int linenoiseHistorySave(const char *filename);
//    int linenoiseHistoryLoad(const char *filename);

    return 0;
}
