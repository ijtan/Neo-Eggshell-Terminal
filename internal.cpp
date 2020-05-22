#include "internal.h"

using namespace std;
vector<string> internalCommands;

struct internalVar {
    string name;
    char *str;
    string value;
};
vector<internalVar> internalVars;

int internalHandler(string command, vector<string> argsV) {
    //will be used to check if the command is internal
    internalCommands.emplace_back("echo");
    internalCommands.emplace_back("showenv");
    internalCommands.emplace_back("unset");
    internalCommands.emplace_back("cd");
    internalCommands.emplace_back("listprocs");
    internalCommands.emplace_back("source");

    for (auto &internalCommand : internalCommands) {
        if (internalCommand == command) {
            switch (&internalCommand - &internalCommands[0]) {
                //TODO MAKE THIS AN ENUM
                case 0:
                    echo(argsV);
                    break;
                case 1:
                    printVars();
                    break;
                case 2:
                    unset(argsV);
                    break;
                case 3:
                    changeDirs(argsV);
                    break;
                case 4:
                    if(getProcs().size()==0)
                        cout<<"No Suspended Processes"<<endl;
                    else {
                        cout << "Listing All suspended Processes" << endl;
                        for (auto pr:getProcs())
                            cout << pr.name<<"\t\t"<<pr.pid << endl;
                    }
                    break;
                case 5:
                    sourceStart(argsV);
                    break;
            }

            return 0;
        }
    }
    return 1;
}

void echo(vector<string> args) {
    int i = 0;
    for (auto arg:args) {
        if (i == 0) {
            i++;
            continue;
        }
        cout << arg << " ";

    }
    cout << endl;
}
int set(vector<string> args) {
    if (args.size() != 1)
        return -1;
    int eq = args[0].find('=');
    if (eq == string::npos) {
        return -1;
    }
    string var = args[0].substr(0, eq);
    string val = args[0].substr(eq + 1, args[0].size() + 1);
    for (char ch:var) {
        if (!(isupper(ch) || !isalpha(ch)))
            return -1;
    }
    if (getenv("var") != NULL) {
        int i = 0;
        for (auto Var:internalVars) {
            if (Var.name == var) {
                internalVars.erase(internalVars.begin() + i);
                break;
            }
            i++;
        }
    }
    char *env = (char*) malloc (var.size() + val.size() + 10);
    sprintf(env, "%s=%s", var.c_str(), val.c_str());

    internalVar newVar = {var,env, val};
    internalVars.push_back(newVar);
    putenv(env); //replace with Assign
    return 0;
//
}

void unset(vector<string> args) {
    if (args.size() == 1) {
        puts("Expected argument to unset");
        return;
    }
    if (args.size() > 2) {
        puts("Expected only one argument");
        return;
    }
    if (getenv(args[1].c_str()) == NULL) {
        puts("Not Found!");
        return;
    } else {
        char tmp[args[1].size()];
        sprintf(tmp,"%s",args[1].c_str());
        putenv(tmp);
    }
}

void changeDirs(vector<string> args) {
    if (args.size() != 2) {
        puts("1 arguments expected: Path");
        return;
    }
    if (chdir(args[1].c_str()) == 0) {
        vector<string>env;
        char envName[255];
        char buf[255];
        getcwd(buf,sizeof(buf));
        sprintf(envName,"CWD=%s",buf);
        env.push_back(envName);
        set(env);
        env.clear();
        return;
    }
    perror("cd");
}
void sourceStart(vector<string> args){
    if(args.size() != 2) {
        puts("1 arguments expected: filename");
        return;
    }
    sourceRun(args[1]);
}
void freeVars(){
    for(const auto& var:internalVars){
        free(var.str);
    }
}