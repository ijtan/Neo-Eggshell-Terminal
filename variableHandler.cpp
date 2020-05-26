#include "variableHandler.h"

using namespace std;

void printVars() {
    for (char **env = environ; *env != NULL; env++) {
        cout << *env << endl;
    }
}

vector<string> parseVars(vector<string> inputVector) {
    int i = 0;
    for (auto in : inputVector) {
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

void initVars(vector<string> &env) {
    if (getenv("PROMPT") == NULL) {
        char prmpt[255];
        sprintf(prmpt, "%s@eggshell> ", getenv("USER"));
        string promp(prmpt);
        better_set("PROMPT", promp);
    }
        char buf[255] = "";
        readlink("/proc/self/exe", buf, sizeof(buf));
        string bf(buf);
        better_set("SHELL", bf);

    if (getenv("CWD") == NULL) {
        char buf[512] = "";
        getcwd(buf,sizeof(buf));
        string CW(buf);
        better_set("CWD",CW);
    }

    if (getenv("TERMINAL") == NULL) {
        char trmnl[255];
        if (isatty(STDIN_FILENO)) {
            better_set("TERMINAL",ttyname(STDIN_FILENO));
        }
    }
}
