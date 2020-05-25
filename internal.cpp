#include "internal.h"
#include <memory>

using namespace std;
extern vector<proc> StpProcs;
vector <string> internalCommands;

struct internalVar {
    string name;
    char *str;
    string value;
};
vector<internalVar> internalVars;

int internalHandler(string command, vector<string> argsV, int exec = 1) {
    //will be used to check if the command is internal
    internalCommands.emplace_back("echo");
    internalCommands.emplace_back("showenv");
    internalCommands.emplace_back("unset");
    internalCommands.emplace_back("cd");
    internalCommands.emplace_back("listprocs");
    internalCommands.emplace_back("source");
    internalCommands.emplace_back("exit");

    for (auto &internalCommand : internalCommands) {
        if (internalCommand == command) {
            switch (&internalCommand - &internalCommands[0]) { //basicaly indexof
                //TODO MAKE THIS AN ENUM
                case 0:
                    echo(argsV);
                    return 0;
                case 1:
                    printVars();
                    return 0;
                case 2:
                    unset(argsV);
                    return 0;
                case 3:
                    changeDirs(argsV);
                    return 0;
                case 4:

                    if(StpProcs.empty())
                        cout<<"No Suspended Processes"<<endl;
                    else {
                        cout << "Listing All suspended Processes" << endl;
                        for (const auto& pr:StpProcs)
                            cout << pr.name<<"\t\t"<<pr.pid << endl;
                    }
                    return 0;
                case 5:
                    sourceStart(argsV);
                    return 0;
            case 6:
                    pid_t parentID = getppid();
                    kill(parentID,SIGINT);
                    return 0;
            }
            return 1;
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
    if (getenv(var.c_str()) != NULL) {
        if(val==getenv(var.c_str())){
            return 0;
        }
        int i = 0;
        for (auto Var:internalVars) {
            if (Var.name == var) {
                free(Var.str);
                internalVars.erase(internalVars.begin() + i);
                break;
            }
            i++;
        }
    }
    char *env = (char*) calloc (var.size() + val.size()+10,sizeof(char));
    snprintf(env,var.size() + val.size()+10, "%s=%s", var.c_str(), val.c_str());

    internalVar newVar = {var,env, val};
    internalVars.emplace_back(newVar);
    putenv(env); //replace with Assign
    return 0;
//
}

int better_set(string variable,string value) {
    size_t size = variable.size() + value.size()+10;
    char *env = (char*) calloc (size, sizeof(char));
    snprintf(env,size, "%s=%s", variable.c_str(), value.c_str());

    internalVar newVar = {variable,env, value};
    internalVars.emplace_back(newVar);
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
    internalVars.clear();
}
void printVarVec(){
    cout<<"VARVEC"<<endl;
    for(const auto& var:internalVars){
        cout<<"Name: "<<var.name<<"; ptr: "<<var.str<<"; ptr&: "<<&var.str<<"; val:"<<var.value<<endl;
    }
}