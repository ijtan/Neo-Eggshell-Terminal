#include "variableHandler.h"

using namespace std;

void printVars() { //prints all environemnt variables using the environ array 
    for (char **env = environ; *env != NULL; env++) {
        cout << *env << endl;
    }
}

void parseVars(vector<string> &input, string &line) { //used to change variable names into values
    int i = 0;
    for (auto in : input) { //for each element in the input array
        if (in.find('$') != string::npos) { //if $ is present ...
            if (in.find('$') > 0) {
                if (in[in.find('$') - 1] == '\\') {//ensure that the $ is not escaped (bad implementation id say, couldve done better, but for this case it works)
                    string escaped = in.erase(in.find('$') - 1, 1); //remove the slash, and continue to next dollar sign if any (obviosly skipping the evaluation parts)
                    input[i] = escaped;
                    i++;
                    continue;
                }
            }
            string envName = in.substr(in.find('$') + 1); //get the envname as the text from the $ sign (non inclusive) until the end of the argument
            if (getenv(envName.c_str()) != NULL) { 
                string newEnv = in.substr(0, in.find('$')); //get the variable name
                newEnv.append(getenv(envName.c_str())); //get value from variable name
                input[i] = newEnv; //set the argument to be the value 

                int pos = line.find('$'); //get position of dollar sign
                int j;
                for (j = pos; j < line.length(); j++) 
                    if (line[j] == ' ')
                        break;
                //get position of agrument end 
                //erase the text from the dollar sign till the next space
                line.erase(pos, j - pos);
                //add the value instead
                line.insert(pos, getenv(envName.c_str()));
            } else
                cout << "The environment for variable '" << envName.c_str() << "' was not found!" << endl; //alert the user about aby errors
        }
        i++; //move on to nect variable if any 
    }
}

void initVars() { //used to intilaise the intial vectors
    string user = string(getenv("USER")); //gets the user from the default user variable
    string promp(user + "@eggshell> ");
    setenv("PROMPT", promp.c_str(), 1); //set prompt
    char shl[128] = "";
    readlink("/proc/self/exe", shl, sizeof(shl)); //get the shell ocation using readlink
    string sh(shl);
    setenv("SHELL", sh.c_str(), 1); //set shell

    char buf[128] = "";
    getcwd(buf, sizeof(buf));
    string CW(buf);
    setenv("CWD", CW.c_str(), 1); //get CWD using the getCWD() and set it accorindlgy

    if (isatty(STDIN_FILENO))
        setenv("TERMINAL", ttyname(STDIN_FILENO), 1); //ensure that stdin is a tty, in which case we set the environemt variable to it.
}
