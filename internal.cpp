#include "internal.h"

using namespace std;
vector<string> internalCommands;

int internalHandler(string command, vector<string> argsV) {
    //will be used to check if the command is internal
    internalCommands.emplace_back("echo");
    internalCommands.emplace_back("showenv");
    internalCommands.emplace_back("unset");

    for (auto &internalCommand : internalCommands) {
        if (internalCommand == command) {
            cout << "Internal command found: " << command << endl;
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

void set(string Assign) {
    putenv("SomeVariable=SomeValue"); //replace with Assign

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
    }
    char *env = const_cast<char *>(args[1].c_str());
    putenv(env); //replace with Assign

}