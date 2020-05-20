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
    printf("ROOT : %s\n", getenv("ROOT"));

    //Take input
    cout<<"Enter Path and command to run"<<endl;
    char *Input = (char*)malloc(MaxCmdLen*sizeof(char));
    //Take Allow for multiple arguments and such, probably implementation with linenoise
    cin >> Input;

    //call function which runs externals commands
    runExt(Input);

    //free the input
    //TODO check if new&delete can be used instead of malloc&free
    free(Input);
    return 0;
}
