#include "internal.h"

using namespace std;
vector<string> internalCommands;

int internalHandler(string command, vector<string>argsV) {
    //will be used to check if the command is internal
    internalCommands.emplace_back("echo");
    internalCommands.emplace_back("showenv");

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
            }

            return 0;
        }
    }
    return 1;
}

void echo(vector<string>args) {
    int i = 0;
    for(auto arg:args) {
        if(i==0){
            i++;
            continue;
        }
        cout << arg << " ";

    }
    cout << endl;
}