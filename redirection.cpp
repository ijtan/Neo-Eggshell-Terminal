#include "redirection.h"

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
    return openRed(STDOUT_FILENO, filename.c_str(), O_SYNC | O_CLOEXEC | O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
}

int append(string filename) {
    return openRed(STDOUT_FILENO, filename.c_str(), O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
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

void BetterSourceRun(string filename) {
    ifstream sourceRead(filename);
    if (!sourceRead.is_open())
        cout << "Failed to open!" << endl;
    cout << "reading" << endl;

    vector<string> args;

    string line;
    string copy;
    char lineC[512];
    char copyC[512];
    while (sourceRead.good() && getline(sourceRead, line)) {
        if (line[0] == '/' && line[1] == '/')
            continue;

        strncpy(lineC, line.c_str(), 512);
        strncpy(copyC, line.c_str(), 512);
        if (tokenize(lineC, copyC, args) == -1) {
            continue;
        };
        if (line.find(filename) == string::npos || line.find("source") == string::npos) {
            cout << "reparsing: " << line << " by: " << getpid() << endl;
            reParse(line, args);
            args.clear();
        } else {
            cerr << "\nSource referring to the same filaneme was found! Skipping to avoid Loop!\n"
                 << endl;
            continue;
        }
    }
    if (sourceRead.bad())
        perror("error while reading file");
    sourceRead.close();
}

int InitialzeRedir(vector<int> conf, vector<string>& args) {
    if (conf[2] == 1) {
        int count = 0;
        int j = 0;
        int argno = 0;

        for (auto arg : args) {
            if (!arg.empty() &&
                arg.length() == 1 && arg == "<") {
                count++;
                j = argno;
            }
            argno++;
        }

        if (count > 1) {
            cout << "Multiple input specifiers found! Aborting..." << endl;
            return -5;
        }
        if (argno < 3) {
            cout << "Command to redirect not specified" << endl;
            return -5;
        }

        //cutting the first 2 characters
        //cout << "zrgno" <<argno<< endl;

        int specifierPos = j;
        int openIN = input(args[specifierPos + 1]);
        if (openIN == -1)
            return -5;
        args.erase(args.begin() + specifierPos, args.begin() + specifierPos + 2);
    }

    if (conf[0] == 1 || conf[1] == 1) {
        if (conf[0] == 1 && conf[1] == 1) {
            cerr << "\n'>' and '>>' cannot be used at the same time! ABorting...\n"
                 << endl;
            return -5;
        }

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
        //after checking the position, we should remove the > filename from the args
        args.erase(args.begin() + j, args.begin() + j + 2);
        if (conf[1] == 1)
            truncOut(args[j + 1]);
        else
            append(args[j + 1]);
    }
    return 0;
}

vector<string> initPipes(vector<string> argV, vector<pid_t>& toWait) {
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
    pid_t PipepPid;
    int outFDtoClose = -10;
    for (int part = 0; part < pipeCount + 1; part++) {
        prevFD = currFD - 2;

        if (part < pipeCount)
            pipe(currFD);
        outFDtoClose = *currFD;
        PipepPid = fork();
        int status = 0;
        signal(SIGUSR1,reSig);


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
            cerr << "\n  returning from redir:" << getpid() << "\n"
                 << endl;
            return argV;
        } else {
            toWait.push_back(PipepPid);
        }
        currFD += 2;
    }
    //waits for the toWait function to be incremented by all children before retutning, this ensures that all children are waited for later
    //as a fail safe this has a max wait of 2 seconds! (example in case the fork fails)
    return {to_string(PipepPid)};
}

void reSig(int signum){
    if(signum == SIGUSR1){
        char msg[255];
        sprintf(msg, "\nRecieved USR SIG! I AM: %d\n",getpid());
        write(STDERR_FILENO, msg,sizeof(msg) );
    }
}

void flagger(string line, vector<int>& RedirectConfig) {
    RedirectConfig[0] = RedirectConfig[1] = RedirectConfig[2] = 0;
    if (line.find(">>") != string::npos)
        RedirectConfig[0] = 1;
    else if (line.find('>') != string::npos) {
        // redirect out
        RedirectConfig[1] = 1;
    }
    if (line.find('<') != string::npos) {
        // redirect in
        RedirectConfig[2] = 1;
    }
}