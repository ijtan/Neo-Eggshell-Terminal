#include "variableHandler.h"

using namespace std;

void printVars() {
    for (char **env = environ; *env != NULL; env++) {
        cout<<*env<<endl;
    }
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
