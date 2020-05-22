#include "parser.h"

//given input, this class will determine some attributes of input such as pipes, redirects etc
// then the respective handler is called
using namespace std;


int parseLine(string line, vector<string> input) {
    int RedirectConfig[5];
    //0: >>
    //1: >
    //2: <
    //3: |
    //4: & -> background run

    if (line.find('=') != string::npos){
        if(input.size()==1){
            if(set(input)==0)
                return 0;
        }
    }
    if (line.find(">>") != string::npos) {
        // redirect append
        cout << "append detected" << endl;
        char ln[line.size()];
        strcpy(ln,line.c_str());
        fpos_t pos;
        int fd;
        fd = dup(fileno(stdout));
        FILE *tmpfile = append(ln,input);
        if(tmpfile!=nullptr) {
            string newl(ln);


            parseLine(newl, input);

            fflush(stdout);
            dup2(fd,fileno(stdout));
            close(fd);
            fsetpos(stdout, &pos);
            //http://c-faq.com/stdio/undofreopen.html
        }
        return 0;
    }
    else if (line.find('>') != string::npos) {
        // redirect out
        cout << "> detected" << endl;
        RedirectConfig[1] = 1;
    }
    if (line.find('<') != string::npos) {
        // redirect in
        cout << "< detected" << endl;
        RedirectConfig[2] = 1;
    }
    if (line.find('&') != string::npos) {
        // background running
        if(input[input.size()-1]=="&"){
            input.erase(input.end());
            cout << "& detected" << endl;
            RedirectConfig[4]=1;
        }

    }
    if (line.find('"') != string::npos) {
        // quotes for combining args
        cout << "\" detected" << endl;
    }

    if (line.find('$') != string::npos) {
        // if var exists replace the word with the value and return value
        input = parseVars(input);
    }
    if (line.find('|') != string::npos) {
        // pipe
        cout << "pipe detected" << endl;
        RedirectConfig[3] = 1;
    }

    if (internalHandler(input[0], input) == 0)
        //checks if the internalHandler matched; meaning that an internal command was run and we do not need further execution
        return 0;
    if(runExt(input, RedirectConfig)==-1){return -1;};
    return 0;

}