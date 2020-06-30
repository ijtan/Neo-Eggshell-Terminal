#include "internal.h"

using namespace std;
//check if process was stopped
vector<string> internalCommands = {"echo", "showenv", "listproc", "source"}; 
vector<string> internalCommandsParentOnly = {"unset", "cd", "bg", "exit"}; //this hold commands which can only be through the main process

int internalChecker(string command) { //this checks is a command exists or not, if it exists it also returns a value signaling if process is internal only or normal
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

int internalHandlerNoCHild(string command, vector<string> argsV) { //if exists executes mehtod correpsonding to the command
    for (auto &cmd : internalCommandsParentOnly) { //loop though all commands
        if (cmd == command) { //if current element is euqal to command
            switch (&cmd - &internalCommandsParentOnly[0]) { //this is like an indexof(). thus we take the relative index of the element, and use it for the switch statement.
                case 0: //unset
                    if (argsV.size() == 2)
                        unsetenv(argsV[1].c_str());
                    else
                        cerr << "Invalid argument size!" << endl;
                    return 0;
                case 1: //cd command
                    changeDirs(argsV);
                    return 0;
                case 2: //bg command
                    resumeStopped();
                    return 0;

                case 3://exit command
                    exit(EXIT_SUCCESS);
                default:
                    cerr << "Internal Command handling error!" << endl;
                    return -1;
            }
        }
    }
    return 1;
}

int internalHandler(string command, vector<string> argsV) { //same thing as before, yet for normal internal commands
    // will be used to check if the command is internal

    for (auto &intCMD : internalCommands) {
        if (intCMD == command) {
            switch (&intCMD - &internalCommands[0]) {  // basicaly indexof
                case 0://echo
                    echo(argsV);
                    return 0;
                case 1://showenv
                    printVars();
                    return 0;
                case 2://listproc
                    listProc();
                    return 0;
                case 3://source command
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
        cerr << "No Suspended Processes" << endl; //if no processes are stopped
    else {
        cout << "Listing All suspended Processes" << endl; //if there are suspended processes
        for (const auto pr : getProcVec())
            cout << pr.name << "\t\t" << pr.pid << endl; //print name and pid 
    }
}

void echo(vector<string> args) { //echo
    int i = 0;
    for (auto arg : args) { //print each element seperated by a space
        if (i == 0) {
            i++;
            continue;
        }
        cout << arg << " ";
    }
    cout << endl; //new line
}

void changeDirs(vector<string> args) {
    if (args.size() != 2) {
        puts("1 argument expected: Path"); //ensure path argument was provided
        return;
    }
    if (chdir(args[1].c_str()) == 0) { //change directory and check if it succeeded
        char buf[512] = "";
        getcwd(buf, sizeof(buf)); //get the new CWD
        setenv("CWD", buf, 1); //update the environemnt variab;e
        return;
    }
    perror("cd"); //inform the user about the error
}