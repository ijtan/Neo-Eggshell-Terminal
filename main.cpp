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
vector<string> env;
void lineReadInit() {
    atexit(exitRoutine);

    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);
    initVars(env);
    //start linenoise loop
    while ((line = linenoise(getenv("PROMPT"))) != NULL) {
        flush(cout);
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

        pid_t callerID = getppid();
        if (pl == -5)
            _exit(-5);
        if(getppid()!=callerID){
            cout<<"Unkilled fork detected, aborting child: ("<<getppid()<<"!="<<callerID<<')'<<endl;
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
    //init vars
    lineReadInit();

    // from documentation:
//    int linenoiseHistorySave(const char *filename);
//    int linenoiseHistoryLoad(const char *filename);

    return 0;
}
