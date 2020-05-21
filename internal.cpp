#include "internal.h"

using namespace std;
vector<string> internalCommands;

struct internalVar{
    char*ptr;string name;string value;
};
vector<internalVar> internalVars;

int internalHandler(string command, vector<string> argsV) {
    //will be used to check if the command is internal
    internalCommands.emplace_back("echo");
    internalCommands.emplace_back("showenv");
    internalCommands.emplace_back("unset");
    internalCommands.emplace_back("cd");

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
    if(args.size()!=1)
        return -1;
    int eq = args[0].find('=');
    if(eq==string::npos){
        return -1;
    }
    string var = args[0].substr(0,eq);
    string val = args[0].substr(eq+1,args[0].size()+1);
    for(char ch:var){
        if(!(isupper(ch)||!isalpha(ch)))
            return -1;
    }
    if(getenv("var")!=NULL) {
        int i = 0;
        for (auto Var:internalVars) {
            if (Var.name == var) {
                free(Var.ptr);
                internalVars.erase(internalVars.begin() + i);
                break;
            }
            i++;
        }
    }
    char *env = static_cast<char *>(malloc(var.size() + val.size() + 10));
    sprintf(env, "%s=%s", var.c_str(), val.c_str());
    internalVar newVar = {env,var,val};
    internalVars.push_back(newVar);
    putenv(newVar.ptr); //replace with Assign
    return 0;

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
    }else{
        int i = 0;
        for (auto Var:internalVars) {
            if (Var.name == args[1]) {
                char *env = const_cast<char *>(args[1].c_str());
                putenv(env);
                internalVars.erase(internalVars.begin() + i);
                free(Var.ptr);
                break;
            }
            i++;
        }
    }
}

void changeDirs(vector<string> args) {
    if (args.size() != 2) {
        puts("1 arguments expected: Path");
        return;
    }
    if (chdir(args[1].c_str()) == 0) {
        char env[args[1].size() + 5];
        sprintf(env, "CWD=%s", args[1].c_str());
        putenv(env);
        return;
    }
    perror("cd");
}
//check if dir exists

void freeInternalVars(){
    for(auto var:internalVars){
        free(var.ptr);
    }
}