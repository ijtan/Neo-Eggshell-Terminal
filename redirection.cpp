#include "redirection.h"

#include <fcntl.h>

#include <fstream>
#include <iostream>

using namespace std;
//REFERENCE: explanation videos
int openRed(int fd, char* path, int flg, mode_t md) {
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

int truncOut(char* filename) {
    return openRed(STDOUT_FILENO, filename, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

int append(char* filename) {
    return openRed(STDOUT_FILENO, filename, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

int input(char* filename) {
    return openRed(STDIN_FILENO, filename, O_RDONLY, S_IRUSR);
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

int InitialzeRedir(int* conf, char** args, int size) {
    if (conf[2] == 1) {
        int count = 0;
        int j = 0;
        int argno = 0;

        for (int i = 0; i < size; i++) {
            if (args[i] != NULL &&
                strlen(args[i]) == 1 &&
                strncmp(args[i], "<", 1) == 0) {
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
            args[1] = NULL;
        } else {
            int k = 0;
            //cout << "moving" << endl;
            while (args[k + 2] != NULL && k < argno) {
                //cout << "moved: " << args[k + 2] << "--into--" << args[k] << endl;
                strcpy(args[k], args[k + 2]);
                //args[k] = args[k + 2];
                k++;
            }
            args[k - 2] = NULL;
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
        for (int i = 0; i < size; i++) {
            if (args[i] != NULL && strlen(args[i]) == len && strncmp(args[i], cmp, len) == 0) {
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
            truncOut(args[argno - 2]);
        else
            append(args[argno - 2]);
        args[argno - 3] = NULL;
    }
}

char* initPipes(int* conf, vector<string> argV, char** args) {
    int i = 0;

    if (conf[3] == 1) {
        //start piping
        int pipeCount = 0;
        char* args2[16][1024];
        int pos = 0;
        int i = -1;
        for (int i = 0; args[i] != NULL && i < 16; i++) {
            if (strncmp(args[pos], "|", 1) == 0) {
                args2[pipeCount][pos] = NULL;
                pipeCount++;
                pos = 0;
                continue;
            }
            args2[pipeCount][pos] = args[i];
            pos++;
        }
        //REFERENCE - CPS1012 - Redirection and Pipes (Sys Prog) Part 2 by Keith Bugeja

        int fd[pipeCount * 2],
            *currFD = fd,
            *prevFD = NULL;

        pid_t callerPID = getpid();

        for (int part = 0; part < pipeCount + 1; part++) {
            prevFD = currFD - 2;

            if (part < pipeCount)
                pipe(currFD);

            cout << "forking: " << part << " by pid " << getpid() << endl;
            pid_t PipepPid = fork();

            if (PipepPid == -1) {
                perror("Pipe fork");
                return args;
            } else if (PipepPid == 0) {
                int cnt = 0;
                for (cnt = 0; args2[part][cnt] != NULL && cnt < pipeCount + 1; cnt++) {
                    args[cnt] = args2[part][cnt];
                }
                args[cnt] = NULL;

                if (part < pipeCount) {
                    cout << "dup out" << part << endl;
                    close(currFD[0]);  // not needed since nothing will be passing to it;
                    dup2(currFD[1], STDOUT_FILENO);
                    close(currFD[1]);  // not needed since is redirected to stdout
                }

                if (part > 0) {
                    cout << "dup in" << part << endl;
                    close(prevFD[1]);
                    dup2(prevFD[0], STDIN_FILENO);
                    close(prevFD[0]);
                }
                currFD += 2;
            }
        }
        return 0;
    }
}