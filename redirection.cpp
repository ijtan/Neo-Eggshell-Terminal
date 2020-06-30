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
                   //call the open redir, with several permissions and flags
                   //the meaning of these can be found in the official documenation
                   //yet these state how the file is to be accesed, and which permission we should give
}

int append(string filename) {
    return openRed(STDOUT_FILENO, filename.c_str(), O_RDWR | O_CREAT | O_APPEND,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); 
                   //refer to previous method
}

int input(string filename) {
    //similar to other methods
    struct stat buffer;
    if (stat(filename.c_str(), &buffer) != 0) {
        cerr << "\nfile not found, Aborting...\n"; //ensure that file provided for input actually exists
        return -1;
    };
    return openRed(STDIN_FILENO, filename.c_str(), O_RDONLY, S_IRUSR);
}

void BetterSourceRun(vector<string> argsV) {
    //chekc that number of arguments match, and thus 'source' and a filename
    if (argsV.size() != 2) {
        puts("1 argument expected: filename");
        _exit(EXIT_FAILURE);
    }
    string filename = argsV[1]; //take the second element in args to be the filena,e
    ifstream sourceRead(filename); //open a stream
    if (!sourceRead.is_open()) {
        cout << "Failed to open!" << endl; //alert the user opn error
        _exit(EXIT_FAILURE);
    }
    //these variables are used for tokenization 
    //this could have been avoided by revamping the tokenize, which wouldnt even be too hard, yet in onrder to complicate things (and keep withing the time limits)
    vector<string> args;

    string line;
    string copy;
    char lineC[512];
    char copyC[512];
    while (sourceRead.good() && getline(sourceRead, line)) { //loop, for eahc line in the source file
        if (line[0] == '/' && line[1] == '/') //if the line starts with a double slash, meaning a comment, skip it 
            continue;
        if (line.empty()) //if line is empty skip it
            continue;
        strncpy(lineC, line.c_str(), 512);
        strncpy(copyC, line.c_str(), 512);
        if (tokenize(lineC, copyC, args) == -1) //tokenize the line 
            continue;

        if (line.find(filename) == string::npos || //this minimises the chance of getting an infinite loop with source files
            line.find("source") == string::npos) {
            cout << "\033[1;31m" << '[' << line << ']' << "\033[0m"
                 << endl;
            reParse(line, args); //call the reparse function which parses and executes everything
            args.clear(); //clear the arg vector
        } else {
            cerr << "\nSource referring to the same filaneme was found! Skipping to "
                    "avoid Loop!\n"
                 << endl;
            continue;
        }
    }
    if (sourceRead.bad()) //ensure we do not have a bad bit, and that the stream is still good
        perror("error while reading file");
    sourceRead.close();
}

int InitialzeRedir(vector<int> conf, vector<string> &args) { //redirection
    int result;
    if (conf[2] == 1) { //if in redirect ('<')
        int count = 0;
        int j = 0;
        int argno = 0;

        for (auto arg : args) {
            if (!arg.empty() && arg.length() == 1 && arg == "<") { //count number of in redirects 
                count++;
                j = argno;
            }
            argno++;
        }
        //report any errors
        if (count != 1) {
            cerr << "Multiple input specifiers found! Aborting..." << endl;
            _exit(EXIT_FAILURE);
        }
        if (argno < 3) {
            cerr << "Missing argument for redirect! Aborting..." << endl;
            _exit(EXIT_FAILURE);
        }
            //get the posiiton of the speicfier, used to remove it later on 
        int specifierPos = j;
        int openIN = input(args[specifierPos + 1]); //actually open the redirect
        args.erase(args.begin() + specifierPos, args.begin() + specifierPos + 2); //erase the '<' argument and the filneame
        if (openIN != 0)
            return openIN; //return
    }
    if (conf[0] == 1 || conf[1] == 1) { //if there are out redirects (append '>>' or '>')
        if (conf[0] == 1 && conf[1] == 1) { //we ensure that both append and turncate outare not used at the same time
            cerr << "\n'>' and '>>' cannot be used at the same time! Aborting...\n"
                 << endl;
            _exit(EXIT_FAILURE);
        }

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
        //check for errors
        if (count != 1) {
            cerr << "Multiple input specifiers found! Aborting..." << endl;
            _exit(EXIT_FAILURE);
        }
        if (argno < 3) {
            cerr << "Missing argument for redirect! Aborting..." << endl;
            _exit(EXIT_FAILURE);
        }
        // after checking the position, we should remove the > filename from the
        // args

        cerr << "Outputting to file: " << args[j + 1] << endl;

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
    //piping time!
    //start two variables which hold information about opened forks, and thus require waiting 
    needWaits PP;
    vector<needWaits> RET;
    // start piping
    int pipeCount = 0;
    vector<vector<string>> splitArgs;
    int i = 0;
    vector<string> temp;
    for (auto arg : argV) { //this prepares the argvector ,which each fork will nedd to individually execute 
        if (arg == "|") {
            splitArgs.push_back(temp);
            temp.clear();
            pipeCount++;
        } else
            temp.push_back(argV[i]);

        i++;
    }
    if (!temp.empty()) //ensure that last lement is always added
        splitArgs.push_back(temp);

    for (auto chunk : splitArgs) { //enusre that no commands are internalParent only, since we alwasy fork when piping
        int intern = internalChecker(chunk[0]);
        if (intern == 1) {
            cerr << "Cannot Pipe One or more internal commands specified. Aborting..."
                 << endl;
            PP.returnCode = 99;
            return {PP};
        }
    } //ensure that there is an arg vector for each pipe+1, thus that splitting of agruments worked as intended
    if (splitArgs.size() != pipeCount + 1) {
        cerr << "Invalid number of commands / pipes. Aborting..." << endl;
        PP.returnCode = -5;
        return {PP};
    }

    // cretae an array of all file descriptos to be used for pipes
    int fd[(pipeCount + 1) * 2], *currFD = fd, *prevFD = NULL;
    pid_t PipePid;
    //loop for each pipe which we need to open
    for (int part = 0; part < pipeCount + 1; part++) {
        prevFD = currFD - 2; //update prevFd to point to the currentFD -2 

        if (part < pipeCount) // if it is not the last pipe
            pipe(currFD);

        PipePid = fork(); //foprk
        int status = 0;

        if (PipePid == -1) { //report fork errors
            perror("Pipe fork");
            _exit(EXIT_FAILURE);
        } else if (PipePid == 0) { //child process
            int cnt = 0;

            // pipe creation
            argV.clear();

            for (auto arg : splitArgs[part]) { //get the processes argVector, and store it as the mainArgv, we can do this since process is independt of all others
                argV.push_back(arg);
                cnt++;
            }
            if (part > 0) { //if not thte first process, link the old process out to current ptocess in 
                close(prevFD[1]);
                dup2(prevFD[0], STDIN_FILENO);
                close(prevFD[0]);
            }
            if (part < pipeCount) { //if not last process, allow current Process out to be linked to, thus linj stdout to it, so ut can be acceed in next iteration with prevFD[]
                close(currFD[0]);  // not needed since nothing will be passing to it;
                dup2(currFD[1], STDOUT_FILENO);
                close(currFD[1]);  // not needed since is redirected to stdout
            }

            PP.returnCode = 0; //return to executor, with the argV and code 0 showing success
            PP.newArgV = argV;
            return {PP};
        } else {
            PP.PID = PipePid; //main process here, set the return object to hold details about the current process
            PP.returnCode = 0;
            PP.PipeCount = pipeCount;
            PP.newArgV = argV;

            RET.push_back(PP); //append details about the lastly forked process into the RET vector

            if (part < pipeCount) { //this c;pses the FD which is not used, this was used to fix a deadlock issue which was encountered, ensured that all pipes are closed. 
                close(currFD[1]);
                currFD += 2; //in crement the currFD
            }
        }
    }

    return RET; //return to executor 
}

void flagger(string line, vector<int> &RedirectConfig) { 
    //quite straight forward, check for redirects or pipes, 
//and raise a flag in the vecotr passed by reference accordingly
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