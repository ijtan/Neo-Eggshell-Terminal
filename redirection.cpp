#include "redirection.h"

using namespace std;

// REFERENCE: explanation videos
int openRed(int fd, const char *path, int flg, mode_t md) {
    // open a new file descriptor at given path
    int FDOpen = open(path, flg, md);

    // check if the FILEDescriptor is the same as the fd in main or it is negative
    // in which case you would return preemptively;
    if (FDOpen == fd || FDOpen < 0)
        return FDOpen;

    // point fd to the opened fd, then close fdopen since it will not be used
    // (at least not through drop but will be used through fd).
    int FDDup = dup2(FDOpen, fd);
    close(FDOpen);

    // make sure the dup2 was successful, if so return fd.
    if (FDDup == -1)
        return -1;
    else
        return 0;
}

int truncOut(string filename) {
    return openRed(STDOUT_FILENO, filename.c_str(),
                   O_SYNC | O_CLOEXEC | O_RDWR | O_CREAT | O_TRUNC,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

int append(string filename) {
    return openRed(STDOUT_FILENO, filename.c_str(), O_RDWR | O_CREAT | O_APPEND,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

int input(string filename) {
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) != 0) {
        cerr << "\nfile not found, Aborting...\n";
        return -1;
    };
    fsync(STDIN_FILENO);
    return openRed(STDIN_FILENO, filename.c_str(), O_RDONLY, S_IRUSR);
}

void BetterSourceRun(vector<string> argsV) {
    if (argsV.size() != 2) {
        puts("1 arguments expected: filename");
        _exit(EXIT_FAILURE);
    }
    string filename = argsV[1];
    ifstream sourceRead(filename);
    if (!sourceRead.is_open()) {
        cout << "Failed to open!" << endl;
        _exit(EXIT_FAILURE);
    }
    cout << "reading" << endl;

    vector<string> args;

    string line;
    string copy;
    char lineC[512];
    char copyC[512];
    while (sourceRead.good() && getline(sourceRead, line)) {
        if (line[0] == '/' && line[1] == '/')
            continue;
        if (line.empty())
            continue;
        sleep(1);
        strncpy(lineC, line.c_str(), 512);
        strncpy(copyC, line.c_str(), 512);
        if (tokenize(lineC, copyC, args) == -1) {
            continue;
        };
        if (line.find(filename) == string::npos ||
            line.find("source") == string::npos) {
            cout << '[' << line << ']' << endl;
            reParse(line, args);
            args.clear();
        } else {
            cerr << "\nSource referring to the same filaneme was found! Skipping to "
                    "avoid Loop!\n"
                 << endl;
            continue;
        }
    }
    if (sourceRead.bad())
        perror("error while reading file");
    sourceRead.close();
}

int InitialzeRedir(vector<int> conf, vector<string> &args) {
    int result;
    if (conf[2] == 1) {
        int count = 0;
        int j = 0;
        int argno = 0;

        for (auto arg : args) {
            if (!arg.empty() && arg.length() == 1 && arg == "<") {
                count++;
                j = argno;
            }
            argno++;
        }
        if (conf[0] == 1 && conf[1] == 1) {
            cerr << "\n'>' and '>>' cannot be used at the same time! ABorting...\n"
                 << endl;
            _exit(EXIT_FAILURE);
        }
        if (count > 1) {
            cerr << "Multiple input specifiers found! Aborting..." << endl;
            _exit(EXIT_FAILURE);
        }
        if (argno < 3) {
            cerr << "Command to redirect not specified" << endl;
            _exit(EXIT_FAILURE);
        }

        int specifierPos = j;
        int openIN = input(args[specifierPos + 1]);
        args.erase(args.begin() + specifierPos, args.begin() + specifierPos + 2);
        if (openIN != 0)
            return openIN;
    }
    if (conf[0] == 1 || conf[1] == 1) {
        char cmp[] = ">>";
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
            cerr << "Multiple input specifiers found! Aborting..." << endl;
            _exit(EXIT_FAILURE);
        }
        // after checking the position, we should remove the > filename from the
        // args

        cerr << "outputting: to file: " << args[j + 1] << endl;

        if (conf[1] == 1)
            result = truncOut(args[j + 1]);
        else
            result = append(args[j + 1]);
        args.erase(args.begin() + j, args.begin() + j + 2);
        if (result != 0)
            return result;
    }
    return 0;
}

vector<needWaits> initPipes(vector<string> argV) {
    needWaits PP;
    vector<needWaits> RET;
    // start piping
    int pipeCount = 0;
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
    if (!temp.empty())
        splitArgs.push_back(temp);

    for (auto chunk : splitArgs) {
        int intern = internalChecker(chunk[0]);
        if (intern == 1) {
            cerr << "Cannot Pipe One or more internal commands specified. Aborting..."
                 << endl;
            PP.returnCode = 99;
            return {PP};
        }
    }
    if (splitArgs.size() != pipeCount + 1) {
        cerr << "Invalid number of commands / pipes. Aborting..." << endl;
        PP.returnCode = -5;
        return {PP};
    }

    // pipe creation
    int fd[(pipeCount + 1) * 2], *currFD = fd, *prevFD = NULL;
    pid_t PipePid;

    for (int part = 0; part < pipeCount + 1; part++) {
        prevFD = currFD - 2;

        if (part < pipeCount)
            pipe(currFD);

        PipePid = fork();
        int status = 0;

        if (PipePid == -1) {
            perror("Pipe fork");
            _exit(EXIT_FAILURE);
        } else if (PipePid == 0) {
            int cnt = 0;

            // pipe creation
            argV.clear();

            for (auto arg : splitArgs[part]) {
                argV.push_back(arg);
                cnt++;
            }
            if (part > 0) {
                close(prevFD[1]);
                dup2(prevFD[0], STDIN_FILENO);
                close(prevFD[0]);
            }
            if (part < pipeCount) {
                close(currFD[0]);  // not needed since nothing will be passing to it;
                dup2(currFD[1], STDOUT_FILENO);
                close(currFD[1]);  // not needed since is redirected to stdout
            }

            PP.returnCode = 0;
            PP.newArgV = argV;
            return {PP};
        } else {
            PP.PID = PipePid;
            PP.returnCode = 0;
            PP.PipeCount = pipeCount;
            PP.newArgV = argV;

            RET.push_back(PP);

            if (part < pipeCount) {
                close(currFD[1]);
                currFD += 2;
            }
        }
    }

    return RET;
}

void flagger(string line, vector<int> &RedirectConfig) {
    RedirectConfig[0] = RedirectConfig[1] = RedirectConfig[2] = RedirectConfig[3] = 0;
    if (line.find(" >> ") != string::npos)
        RedirectConfig[0] = 1;
    else if (line.find(" > ") != string::npos) {
        // redirect out
        RedirectConfig[1] = 1;
    }
    if (line.find(" < ") != string::npos) {
        // redirect in
        RedirectConfig[2] = 1;
    }
    if (line.find(" | ") != string::npos) {
        // redirect in
        RedirectConfig[3] = 1;
    }
}