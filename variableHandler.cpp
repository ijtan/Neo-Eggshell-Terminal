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
void initVars(vector<string> &env){
    env.clear();

    char prmpt[255];
    char usr[255];
    strncpy(usr,getenv("USER"),sizeof(usr));
    sprintf(prmpt, "PROMPT=%s@eggshell> ",usr);
    env.emplace_back(prmpt);
    set(env);
    env.clear();

    char shll[255];
    char buf[255] = "";
    readlink("/proc/self/exe", buf, sizeof(buf));
    sprintf(shll,"SHELL=%s",buf);
    env.push_back(shll);
    set(env);
    env.clear();
    char cwd[255];
    if(getenv("CWD")==NULL){
        getcwd(buf,sizeof(buf));
        sprintf(cwd,"CWD=%s",buf);
        env.push_back(cwd);
        set(env);
        env.clear();
    }
    char trmnl[255];
    if(getenv("TERMINAL")==NULL){
        if(isatty(STDIN_FILENO)){
            sprintf(trmnl,"TERMINAL=%s",ttyname(STDIN_FILENO));
            env.push_back(trmnl);
            set(env);
            env.clear();
        }

    }

}