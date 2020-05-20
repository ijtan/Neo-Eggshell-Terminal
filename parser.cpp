#include "parser.h"
#include "external.h"
//given input, this class will determine some attributes of input such as pipes, redirects etc
// then the respective handler is called
using namespace std;


int parseLine(string line, char **input) {
    //check for pipes
    //check for redirection
    //strtok was recommended, check what it does ! -> not here
    string in(input[0]);


    if (internalHandler(in) != 0) {
        puts("External Command detected !");
        runExt(input);
        cout << "LINE: " <<line<<endl;
    }
    if (line.find('>') != string::npos) {
        // redirect out
        cout << "> detected" << endl;
    }
    if (line.find('<') != string::npos) {
        // redirect in
        cout << "< detected" << endl;
    }
    if (line.find(">>") != string::npos) {
        // redirect append
        cout << "append detected" << endl;
    }
    if (line.find('|') != string::npos) {
        // pipe
        cout << "pipe detected" << endl;
    }
    if (line.find('&') != string::npos) {
        // background running
        cout << "& detected" << endl;
    }
    cout << "no special characters!" << endl;
    return 0;

}