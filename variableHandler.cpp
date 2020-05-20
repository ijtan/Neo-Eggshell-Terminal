#include "variableHandler.h"

using namespace std;

vector<string> vars;

void initVars() {
    vars.emplace_back("PATH");
    vars.emplace_back("HOME");
    vars.emplace_back("USER");
    vars.emplace_back("PWD");
    vars.emplace_back("SHELL");
    vars.emplace_back("PROMPT");
    vars.emplace_back("EXITCODE");
    vars.emplace_back("TERMINAL");
}

void printVars() {
    for (auto &&var:vars) {
        cout << var << "=" << getenv(var.c_str()) << endl;
    }
}

vector<string> getVars() {
    return vars;
}

char **parseVars(char **input) {
    vector<int> indexes;
    int i = 0;
    while (input[i] != NULL) {
        string in(input[i]);
        if (in.find('$') == string::npos) {
            i++;
            continue;
        }
        if (in.find('$') > 0) {
            if (in[in.find('$') - 1] == '\\') {
                string escaped = in.erase(in.find('$') - 1,1);
                strcpy(input[i], escaped.c_str());
                i++;
                continue;
            }
        }
        string envName = in.substr(in.find('$') + 1, in.length());
        if (getenv(envName.c_str()) != NULL) {
            string newEnv = in.substr(0, in.find('$')) + getenv(envName.c_str());
            strcpy(input[i], newEnv.c_str());
        } else
            cout << "The environment for variable '" << envName.c_str() << "' was not found!" << endl;
        i++;
    }
    return input;
}
