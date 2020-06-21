#include "variableHandler.h"

using namespace std;

void printVars() {
    for (char **env = environ; *env != NULL; env++) {
        cout << *env << endl;
    }
}

void parseVars(vector<string> &input, string &line) {
    int i = 0;
    for (auto in : input) {
        if (in.find('$') != string::npos) {
            if (in.find('$') > 0) {
                if (in[in.find('$') - 1] == '\\') {
                    string escaped = in.erase(in.find('$') - 1, 1);
                    input[i] = escaped;
                    i++;
                    continue;
                }
            }
            string envName = in.substr(in.find('$') + 1);
            if (getenv(envName.c_str()) != NULL) {
                string newEnv = in.substr(0, in.find('$'));
                newEnv.append(getenv(envName.c_str()));
                input[i] = newEnv;

                int pos = line.find('$');
                int j;
                for (j = pos; j < line.length(); j++)
                    if (line[j] == ' ')
                        break;

                line.erase(pos, j - pos);
                line.insert(pos, getenv(envName.c_str()));
            } else
                cout << "The environment for variable '" << envName.c_str() << "' was not found!" << endl;
        }
        i++;
    }
}

void initVars(vector<string> &env) {
    if (getenv("PROMPT") == NULL) {
        char prmpt[255];
        sprintf(prmpt, "%s@eggshell> ", getenv("USER"));
        string promp(prmpt);
        setenv("PROMPT", promp.c_str(), 1);
    }
    char buf[255] = "";
    readlink("/proc/self/exe", buf, sizeof(buf));
    string bf(buf);
    setenv("SHELL", bf.c_str(), 1);

    if (getenv("CWD") == NULL) {
        char buf[512] = "";
        getcwd(buf, sizeof(buf));
        string CW(buf);
        setenv("CWD", CW.c_str(), 1);
    }

    if (getenv("TERMINAL") == NULL) {
        if (isatty(STDIN_FILENO))
            setenv("TERMINAL", ttyname(STDIN_FILENO), 1);
    }
}
