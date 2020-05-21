#include "variableHandler.h"

using namespace std;

vector<string> vars;

void initVars() {
    vars.emplace_back("PATH");
    vars.emplace_back("HOME");
    vars.emplace_back("USER");
    vars.emplace_back("PWD");
    vars.emplace_back("CWD");
    vars.emplace_back("SHELL");
    vars.emplace_back("PROMPT");
    vars.emplace_back("EXITCODE");
    vars.emplace_back("TERMINAL");
}

void printVars() {
    for (auto &&var:vars) {
        if(getenv(var.c_str()) == nullptr){
            cout << var << "=NULL" << endl;
        }
        else
            cout << var << "=" << getenv(var.c_str()) << endl;
    }
}

vector<string> getVars() {
    return vars;
}

vector<string>parseVars(vector<string> inputVector) {
    int i = 0;
    for (auto in:inputVector) {
        if (in.find('$') == string::npos) {
            i++;
            continue;
        }
        if (in.find('$') > 0) {
            if (in[in.find('$') - 1] == '\\') {
                string escaped = in.erase(in.find('$') - 1, 1);
                inputVector[i] = escaped;
                i++;
                continue;
            }
        }
        string envName = in.substr(in.find('$') + 1, in.length());
        if (getenv(envName.c_str()) != NULL) {
            string newEnv = in.substr(0, in.find('$'));
            newEnv.append(getenv(envName.c_str()));
            inputVector[i] = newEnv;
        } else
            cout << "The environment for variable '" << envName.c_str() << "' was not found!" << endl;
        i++;
    }
    return inputVector;
}
