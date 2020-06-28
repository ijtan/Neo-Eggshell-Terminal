#include "internal.h"

using namespace std;

vector<string> internalCommands = {"echo", "showenv", "listproc", "source"};
vector<string> internalCommandsParentOnly = {"unset", "cd", "bg", "exit"};

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
                        unsetenv(argsV[1].c_str());
                    else
                        cerr << "Invalid argument size!" << endl;
                    return 0;
                case 1:
                    changeDirs(argsV);
                    return 0;
                case 2:
                    resumeStopped();
                    return 0;

                case 3:
                    exit(EXIT_SUCCESS);
                default:
                    cerr << "Internal Command handling error!" << endl;
                    return -1;
            }
        }
    }
    return 1;
}

int internalHandler(string command, vector<string> argsV) {
    // will be used to check if the command is internal

    for (auto &intCMD : internalCommands) {
        if (intCMD == command) {
            switch (&intCMD - &internalCommands[0]) {  // basicaly indexof
                // TODO MAKE THIS AN ENUM
                case 0:
                    echo(argsV);
                    return 0;
                case 1:
                    printVars();
                    return 0;
                case 2:
                    listProc();
                    return 0;
                case 3:
                    BetterSourceRun(argsV);
                    return 0;
                default:
                    cerr << "Internal Command handling error!" << endl;
                    return -1;
            }
        }
    }
    return 1;
}

void listProc() {
    if (getFirstProc().pid == -1)
        cerr << "No Suspended Processes" << endl;
    else {
        cout << "Listing All suspended Processes" << endl;
        for (const auto pr : getProcVec())
            cout << pr.name << "\t\t" << pr.pid << endl;
    }
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

void changeDirs(vector<string> args) {
    if (args.size() != 2) {
        puts("1 argument expected: Path");
        return;
    }
    if (chdir(args[1].c_str()) == 0) {
        char buf[512] = "";
        getcwd(buf, sizeof(buf));
        setenv("CWD", buf, 1);
        return;
    }
    perror("cd");
}