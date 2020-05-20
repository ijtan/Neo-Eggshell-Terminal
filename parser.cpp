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
    int redir = 0;
    if (line.find('&') != string::npos) {
        // background running
        cout << "& detected" << endl;
    }
    if (line.find('"') != string::npos) {
        // quotes for combining args
        cout << "\" detected" << endl;
    }
    if (line.find('$') != string::npos) {
        // if var exists replace the word with the value and return value
        parseVars(input);
    }
    if (line.find('>') != string::npos) {
        // redirect out
        cout << "> detected" << endl;
    }
    if (line.find('<') != string::npos) {
        // redirect in
        cout << "< detected" << endl;
        redir = 1;
    }
    if (line.find(">>") != string::npos) {
        // redirect append
        cout << "append detected" << endl;
        redir = 1;
    }
    if (line.find('|') != string::npos) {
        // pipe
        cout << "pipe detected" << endl;
        redir = 1;
    }

    if (internalHandler(in,input) == 0)
        return 0;
    if (redir == 0)
        runExt(input);
    if (redir == 1)
        string output = runExtRedir(input);
    cout << "LINE: " <<line<<endl;

    return 0;

}