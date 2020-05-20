//
// Created on 5/20/2020.
//

#include "internal.h"
char **internalCommands;
int internalHandler(char *command){
    //will be used to check if the command is internal
    int i = 0;
    while (i-- > internalCommands.length) {
        if(strcmp(internalCommands[i],command[0]) == 0){
            //handle internal -> execute
            puts("Internal Command found!");
            return 0;
        }
    }
    return 1;
}