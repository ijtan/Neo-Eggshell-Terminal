#include "redirection.h"

#include <fcntl.h>

#include <fstream>
#include <iostream>

using namespace std;
//REFERENCE: explanation videos
int openRed(int fd, const char* path, int flg, mode_t md) {
    //open a new file descriptor at given path
    int FDOpen = open(path, flg, md);

    //check if the FILEDescriptor is the same as the fd in main or it is negative
    //in which case you would return preemptively;
    if (FDOpen == fd || FDOpen < 0)
        return FDOpen;

    //point fd to the opened fd, then close fdopen since it will not be used
    // (at least not through drop but will be used through fd).
    int FDDup = dup2(FDOpen, fd);
    close(FDOpen);

    //make sure the dup2 was successful, if so return fd.
    if (FDDup == -1)
        return -1;
    else
        return fd;
}

int truncOut(string filename) {
    return openRed(STDOUT_FILENO, filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

int append(string filename) {
    return openRed(STDOUT_FILENO, filename.c_str(), O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

int input(string filename) {
    return openRed(STDIN_FILENO, filename.c_str(), O_RDONLY, S_IRUSR);
}

void sourceRun(string filename) {
    ifstream sourceRead(filename);
    if (!sourceRead.is_open())
        cout << "Failed to open!" << endl;
    cout << "reading" << endl;
    char ch;
    ch = sourceRead.get();
    vector<string> args;

    char* command = (char*)calloc(10000, sizeof(char));
    int i = 0;

    while (ch != EOF) {
        if (ch != '\n')
            command[i] = ch;
        else {
            command[i] = '\0';
            char copy[sizeof(command) + 10000] = "";
            strncpy(copy, command, sizeof(copy));
            if (tokenize(command, copy, args) == -1) {
                continue;
            };
            reParse(command, args);
            args.clear();
            memset(command, '\0', strlen(command) * sizeof(char));  // seems to reset the command thingy
            i = -1;
        }

        i++;
        ch = sourceRead.get();
    }
    free(command);
    sourceRead.close();
}

int InitialzeRedir(int* conf, vector<string>& args) {
    if (conf[2] == 1) {
        int count = 0;
        int j = 0;
        int argno = 0;

        for (auto arg : args) {
            if (args.empty() &&
                args.size() == 1 && arg == "<") {
                count++;
                j = argno;
            }
            argno++;
        }

        if (count > 1) {
            cout << "Multiple input specifiers found! Aborting..." << endl;
            return -5;
        }

        if (j != 1) {
            cout << "Input specifier position invalid! Aborting..." << endl;
            return -5;
        }
        if (argno < 3) {
            cout << "Command to redirect not specified" << endl;
            return -5;
        }

        //cutting the first 2 characters
        //cout << "zrgno" <<argno<< endl;

        input(args[0]);
        if (argno == 4) {
            args[0] = args[2];
            //args[1] = nullptr;
        } else {
            int k = 0;
            //cout << "moving" << endl;
            while (!args[k + 2].empty() && k < argno) {
                //cout << "moved: " << args[k + 2] << "--into--" << args[k] << endl;
                args[k] = args[k + 2];
                //args[k] = args[k + 2];
                k++;
            }
            //args[k - 2] = nullptr;
        }
    }

    if (conf[0] == 1 || conf[1] == 1) {
        char cmp[5] = ">>";
        int len = 2;
        if (conf[1] == 1) {
            strcpy(cmp, ">");
            len = 1;
        }

        int count = 0;
        int j = 0;
        int argno = 0;
        for (auto arg : args) {
            if (!arg.empty() && arg.size() == len && arg == cmp) {
                count++;
                j = argno;
            }
            argno++;
        }
        if (count > 1) {
            cout << "Multiple input specifiers found! Aborting..." << endl;
            return -5;
        }
        if (j != argno - 3) {
            cout << "Output specifier position invalid! Aborting..." << endl;
            return -5;
        }
        if (conf[1] == 1)
            truncOut(args[argno - 2].c_str());
        else
            append(args[argno - 2]);
        //args[argno - 3] = nullptr;
    }
    return 0;
}

vector<string> initPipes(vector<string> argV) {
    //start piping
    int pipeCount = 0;
    cout << "starting pipe identification" << endl;
    vector<vector<string>> splitArgs;
    int i = 0;
    vector<string> temp;
    for (auto arg : argV) {
        if (arg == "|") {
            splitArgs.push_back(temp);
            temp.clear();
            pipeCount++;
        } else {
            temp.push_back(argV[i]);
        }
        i++;
    }
    splitArgs.push_back(temp);

    //var creation
    int fd[pipeCount * 2],
        *currFD = fd,
        *prevFD = NULL;

    for (int part = 0; part < pipeCount + 1; part++) {
        prevFD = currFD - 2;

        if (part < pipeCount)
            pipe(currFD);

        pid_t PipepPid = fork();

        if (PipepPid == -1) {
            perror("Pipe fork");
            return {{"-5"}};
        } else if (PipepPid == 0) {

            int cnt = 0;

            argV.clear();
            for (auto arg : splitArgs[part]) {
                argV.push_back(arg);
                cnt++;
            }

            if (part < pipeCount) {
                close(currFD[0]);  // not needed since nothing will be passing to it;
                dup2(currFD[1], STDOUT_FILENO);
                close(currFD[1]);  // not needed since is redirected to stdout
            }
            if (part > 0) {
                close(prevFD[1]);
                dup2(prevFD[0], STDIN_FILENO);
                close(prevFD[0]);
            }
            return argV;
        }
        currFD += 2;
    }
    return {"100"};
}