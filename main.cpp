#include <iostream>
#include <cstdlib>
#include "linenoise.h"
#include "parser.h"
#include "external.h"
#include "internal.h"

using namespace std;
#define MaxCmdLen 100
#define MAX_ARGS 255
#define MAX_HISTORY 20
#define MAX_INTERNAL_CMDS 100


int main() {
    cout << "Welcome to EggShell!" << endl;

    //init vars
    char *line,
            *token = NULL,
            *args[MAX_ARGS];
    int tokenIndex;
    char buff[100];
    sprintf(buff, "PROMPT=%s@eggshell> ",getenv("USER"));
    putenv(buff);

    //init linenoise
    linenoiseHistorySetMaxLen(MAX_HISTORY);



    //TODO chedck that the following are valid and where they are to be used
    puts("\n\nPrinting required env variables");
    printf("PATH : %s\n", getenv("PATH"));
    printf("HOME : %s\n", getenv("HOME"));
    printf("USER : %s\n", getenv("USER"));
    printf("PWD : %s\n", getenv("PWD"));
    printf("SHELL : %s\n", getenv("SHELL"));
    printf("PROMPT : %s\n", getenv("PROMPT"));
    printf("EXITCODE : %s\n", getenv("EXITCODE"));
    printf("TERMINAL : %s\n\n\n", getenv("TERMINAL"));


    //start linenoise loop
    //TODO change runExt to parsing, which will handle all inputs accordingly
    while ((line = linenoise(getenv("PROMPT"))) != NULL) {
        linenoiseHistoryAdd(line);
        token = strtok(line, " ");
        for (tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1; tokenIndex++) {
            args[tokenIndex] = token;
            token = strtok(NULL, " ");
        }

        // set last token to NULL
        args[tokenIndex] = NULL;

        if (strcmp(args[0], "exit") == 0) {
            //TODO MAKE THIS INTERNAL AND WAY TO FREE ALL VARS
            linenoiseFree(line);
            exit(2);
        }
        //call function which runs externals commands

        string str(line, line + strlen(line));
        parseLine(line, args);
        linenoiseFree(line);



    }

    // from documentation:
//    int linenoiseHistoryAdd(const char *line);
//    int linenoiseHistorySetMaxLen(int len);
//    int linenoiseHistorySave(const char *filename);
//    int linenoiseHistoryLoad(const char *filename);

    //TODO check if new&delete can be used instead of malloc&free
    closeInternals();
    return 0;
}
