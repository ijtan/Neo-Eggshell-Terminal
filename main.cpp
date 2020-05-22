#include <iostream>
#include <cstdlib>
#include <cstring>
#include "ext/linenoise.h"
#include "parser.h"
#include "signalHandler.h"



using namespace std;
#define MAX_ARGS 100
#define MAX_HISTORY 25
//extern char**environ;

void initVars(vector<string> &env){
    char buf[255];
    char envName[255];
    sprintf(envName, "PROMPT=%s@eggshell> ", getenv("USER"));
    env.push_back(envName);
    set(env);
    env.clear();
    readlink("/proc/self/exe", buf, sizeof(buf));
    sprintf(envName,"SHELL=%s",buf);
    env.push_back(envName);
    set(env);
    env.clear();
    if(getenv("CWD")==NULL){
        getcwd(buf,sizeof(buf));
        sprintf(envName,"CWD=%s",buf);
        env.push_back(envName);
        set(env);
        env.clear();
    }



}
int tokenize(char*line, char* copy, vector<string> &args){
    char *token=NULL;
    int tokenIndex;
    if (strlen(line)==0) {
        free(line);
        args.clear();
        return -1;
    }
    token = strtok((char *) copy, " ");
    for (tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1; tokenIndex++) {
        args.emplace_back(token);
        token = strtok(NULL, " ");
    }
    return 0;
}
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
            exit(EXIT_SUCCESS);

        }
        if(getenv("PROMPT")==NULL or getenv("SHELL")==NULL) {
            initVars(env);
            prompt = getenv("PROMPT");
        }
    }
}


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
