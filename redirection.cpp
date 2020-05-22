#include <iostream>
#include <fstream>


#include "redirection.h"

using namespace std;

void truncOut(string filename, string content) {
    FILE *out = freopen(filename.c_str(),"w",stdout);
}

FILE* append(char * lin, vector<string> &args) {
    int n = args.size()-1;
    string line(lin);
    //[-2]          [-1]    [-0]
    //EXEC/ARGS     >>      FILENAME.txt
    FILE *RET;
    if(args[n-1]!=">>"){
        fputs("Redirect not where expected\n",stderr);
        return nullptr;
    }else {

        RET = freopen(args[n].c_str(), "a", stdout);
    }
        args.erase(args.end() - 1, args.end() + 1);
        line = line.substr(0, line.find(">>"));
        strcpy(lin,line.c_str());
        return RET;
}

string input(string filename) {
    FILE *reader = freopen(filename.c_str(),"r",stdin);
    return NULL;

}

void sourceRun(string filename){
    ifstream sourceRead(filename);
    if(!sourceRead.is_open())
        cout<<"Failed to open!"<<endl;
    cout<<"reading"<<endl;
    char ch;
    ch = sourceRead.get();
    vector<string> args;


    char* command = (char*)malloc(1024*sizeof(char));
    int i = 0;
    while(ch!=EOF){
        if(i==0){
            char* command = (char*)malloc(1024*sizeof(char));
        }
        cout<<"read: "<<ch<<endl;
        if(ch!='\n')
        command[i]=ch;
        else{
            cout<<"new n: "<<endl;
            char copy[sizeof(command)];
            strcpy(copy,command);
            if(tokenize(command, copy, args)==-1){ continue;};
            cout<<"execing: ["<<command<<"]"<<endl;
            reParse(command,args);

            free(command);
            args.clear();
            i=-1;
        }

        i++;
        ch = sourceRead.get();
    }

    sourceRead.close();
}