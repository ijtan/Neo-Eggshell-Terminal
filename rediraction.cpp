#include <iostream>


#include "rediraction.h"

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
