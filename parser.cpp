#include "parser.h"
#include "external.h"
//given input, this class will determine some attributes of input such as pipes, redirects etc
// then the respective handler is called
using namespace std;


int parseLine(string line, vector<string> input) {
    //check for pipes
    //check for redirection
    //strtok was recommended, check what it does ! -> not here
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
        input = parseVars(input);
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

    if (internalHandler(input[0],input) == 0)
        return 0;
    if (redir == 0)
        runExt(input);
    if (redir == 1)
        cout << "REDIR OUT: "<<runExtRedir(input)<<endl;
    cout << "LINE: " <<line<<endl;

    return 0;

}