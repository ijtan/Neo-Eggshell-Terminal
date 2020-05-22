#include <iostream>
#include <fstream>
#include <fcntl.h>


#include "redirection.h"

using namespace std;
//REFERENCE: explanation videos
int openRed(int fd, char *path, int flg, mode_t md){
    //open a new file descriptor at given path
    int FDOpen = open(path,flg, md);

    //check if the FILEDescriptor is the same as the fd in main or it is negative
    //in which case you would return preemptively;
    if(FDOpen==fd||FDOpen<0)
        return FDOpen;

    //point fd to the opened fd, then close fdopen since it will not be used
    // (at least not through drop but will be used through fd).
    int FDDup = dup2(FDOpen,fd);
    close(FDOpen);

    //make sure the dup2 was successful, if so return fd.
    if(FDDup==-1)
        return -1;
    else
        return fd;
}

int truncOut(char* filename) {
    return openRed(STDOUT_FILENO, filename, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
}

int append(char* filename) {
//    int n = args.size()-1;
//    string line(lin);
//    //[-2]          [-1]    [-0]
//    //EXEC/ARGS     >>      FILENAME.txt
//    FILE *RET;
//    if(args[n-1]!=">>"){
//        fputs("Redirect not where expected\n",stderr);
//        return nullptr;
//    }else {
//
//        RET = freopen(args[n].c_str(), "a", stdout);
//    }
//        args.erase(args.end() - 1, args.end() + 1);
//        line = line.substr(0, line.find(">>"));
//        strcpy(lin,line.c_str());
//        return RET;
    return openRed(STDOUT_FILENO, filename, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
}

int input(char* filename) {
    char Input[10000];
    return openRed(STDIN_FILENO, filename, O_RDONLY,S_IRUSR);

}

void sourceRun(string filename){
    ifstream sourceRead(filename);
    if(!sourceRead.is_open())
        cout<<"Failed to open!"<<endl;
    cout<<"reading"<<endl;
    char ch;
    ch = sourceRead.get();
    vector<string> args;


    char* command = (char*)calloc(10000,sizeof(char));
    int i = 0;

    while(ch!=EOF){
        if(ch!='\n')
            command[i]=ch;
        else{
            command[i]='\0';
            char copy[sizeof(command)+10000] = "";
            strncpy(copy,command,sizeof(copy));
            if(tokenize(command, copy, args)==-1){ continue;};
            cout<<"Executing: ["<<command<<"]"<<endl;
            reParse(command,args);
            args.clear();
            memset(command,'\0',strlen(command)*sizeof(char)); // seems to reset the command thingy
            i=-1;
        }

        i++;
        ch = sourceRead.get();
    }
    free(command);
    sourceRead.close();
}