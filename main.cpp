#include <iostream>
#include <stdlib.h>
#include "linenoise.h"
#include "external.h"
using namespace std;
#define MaxCmdLen 100
int main()
{
    //TODO implement linenoise!
    cout<<"Welcome to EggShell!"<<endl;

    //TODO chedck that the following are valid and where they are to be used
    printf("PATH : %s\n", getenv("PATH"));
    printf("HOME : %s\n", getenv("HOME"));
    printf("ROOT : %s\n\n\n", getenv("ROOT"));
    flush(cout);

    //Take input
    char *line = (char *) malloc(MaxCmdLen*sizeof(char));
    char *name = getenv("LOGNAME");
    char *prompt = strcat(name,"> ");

    while((line = linenoise(prompt)) != NULL) {
        cout << "You wrote: "<< line << endl;

        if(strcmp(line, "exit") == 0){
            free(line);
            exit(2);
        }
        //call function which runs externals commands
        runExt(line);



    }

    // from documentation:
//    int linenoiseHistoryAdd(const char *line);
//    int linenoiseHistorySetMaxLen(int len);
//    int linenoiseHistorySave(const char *filename);
//    int linenoiseHistoryLoad(const char *filename);



    //Take Allow for multiple arguments and such, probably implementation with linenoise




    //free the input
    //TODO check if new&delete can be used instead of malloc&free
    return 0;
}
