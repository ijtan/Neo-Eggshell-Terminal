#include "internal.h"

using namespace std;
char **internals = (char**) malloc(MAX_INTERNAL_CMDS*MaxCmdLen*sizeof(char));

int internalHandler(char *command){
    internals[0] = "echo";
    //will be used to check if the command is internal

    for(int i = 0; i<MAX_INTERNAL_CMDS; i++ ) {
        if(strcmp(internals[i],command)==0){
            //handle internal -> execute
            puts("Internal Command found!");
            return 0;
        }
    }
    return 1;
}
void closeInternals(){
    free(internals);
}