#include "internal.h"

using namespace std;

vector<string> internalCommands = {"echo", "showenv", "listproc", "source"};
vector<string> internalCommandsParentOnly = {"unset", "cd", "bg", "exit"};

struct internalVar {
    string name;
    char *str;
    string value;
};

vector<internalVar> internalVars;

int internalChecker(string command) {
    for (auto childOK : internalCommands) {
        if (command == childOK)
            return 0;
    }
    for (auto noChild : internalCommandsParentOnly) {
        if (command == noChild)
            return 1;
    }
    return -1;
}

int internalHandlerNoCHild(string command, vector<string> argsV) {
    for (auto &cmd : internalCommandsParentOnly) {
        if (cmd == command) {
            switch (&cmd - &internalCommandsParentOnly[0]) {
                case 0:
                    if (argsV.size() == 2)
                        better_unset(argsV[1]);
                    else
                        cout << "Invalid argument size!" << endl;
                    return 0;
                case 1:
                    changeDirs(argsV);
                    return 0;
                case 2:
                    resumeStopped();
                    return 0;

                case 3:
                    exit(EXIT_SUCCESS);
            }
        }
    }
    return 1;
}

int internalHandler(string command, vector<string> argsV) {
    // will be used to check if the command is internal

    for (auto &internalCommand : internalCommands) {
        if (internalCommand == command) {
            switch (&internalCommand - &internalCommands[0]) {  // basicaly indexof
                // TODO MAKE THIS AN ENUM
                case 0:
                    echo(argsV);
                    return 0;
                case 1:
                    printVars();
                    return 0;
                case 2:
                    if (getFirstProc().pid == -1)
                        cout << "No Suspended Processes" << endl;
                    else {
                        cout << "Listing All suspended Processes" << endl;
                        for (const auto pr : getProcVec())
                            cout << pr.name << "\t\t" << pr.pid << endl;
                    }
                    return 0;
                case 3:
                    sourceStart(argsV);
                    return 0;
            }
        }
    }
    return 1;
}

void echo(vector<string> args) {
    int i = 0;
    for (auto arg : args) {
        if (i == 0) {
            i++;
            continue;
        }
        cout << arg << " ";
    }
    cout << endl;
}

int better_set(string variable, string value) {
    for (auto var : internalVars) {
        if (var.str == variable)
            better_unset(variable);
    }

    size_t size = variable.size() + value.size() + 10;
    char *env = (char *)calloc(size, sizeof(char));
    snprintf(env, size, "%s=%s", variable.c_str(), value.c_str());

    internalVar newVar = {variable, env, value};
    internalVars.emplace_back(newVar);
    putenv(env);
    return 0;
}

void better_unset(string variable) {
    if (getenv(variable.c_str()) == NULL) {
        puts("Not Found!");
        return;
    }
    unsetenv(variable.c_str());
    int i = 0;
    for (auto var : internalVars) {
        if (var.name == variable) {
            cout << "erasing: " << (internalVars[i]).name << endl;
            free(var.str);
            internalVars.erase(internalVars.begin() + i);
            break;
        }
        i++;
    }
}

void changeDirs(vector<string> args) {
    if (args.size() != 2) {
        puts("1 arguments expected: Path");
        return;
    }
    if (chdir(args[1].c_str()) == 0) {
        char buf[512] = "";
        getcwd(buf, sizeof(buf));
        string CW(buf);
        better_set("CWD", CW);
        return;
    }
    perror("cd");
}

int sourceStart(vector<string> args) {
    if (args.size() != 2) {
        puts("1 arguments expected: filename");
        return -5;
    }
    return BetterSourceRun(args[1]);
}

void freeVars() {
    for (const auto &var : internalVars) {
        free(var.str);
    }
    internalVars.clear();
}

void printVarVec() {
    cout << "VARVEC" << endl;
    for (const auto &var : internalVars) {
        cout << "Name: " << var.name << "; ptr: " << var.str
             << "; ptr&: " << &var.str << "; val:" << var.value << endl;
    }
}