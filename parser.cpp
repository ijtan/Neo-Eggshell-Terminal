#include "parser.h"
#include "external.h"
//given input, this class will determine some attributes of input such as pipes, redirects etc
// then the respective handler is called
using namespace std;


int parseLine(string line, char **input){
    //check for pipes
    //check for redirection
    //strtok was recommended, check what it does ! -> not here

    if(internalHandler(input[0]) != string::npos) {
        if (line.find(">") != string::npos) {
            // redirect out
        }
        if (line.find("<") != string::npos) {
            // redirect in
        }
        if (line.find(">>") != string::npos) {
            // redirect append
        }
        if (line.find("|") != string::npos) {
            // pipe
        }
        if (line.find("&") != string::npos) {
            // background running
        }
        return 1;
    }
    puts("External Command detected !");
    runExt(input);
    return 0;

}