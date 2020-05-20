#include "parser.h"

//given input, this class will determine some attributes of input such as pipes, redirects etc
// then the respective handler is called
int classify(char *input){
    //check for pipes
    //check for redirection
    //strtok was recommended, check what it does ! -> not here
    if (strstr(Input, ">") != NULL) {
        // redirect out
    }
    if (strstr(Input, "<") != NULL) {
        // redirect in
    }
    if (strstr(Input, ">>") != NULL) {
        // redirect append
    }
    if (strstr(Input, "|") != NULL) {
        // pipe
    }
}