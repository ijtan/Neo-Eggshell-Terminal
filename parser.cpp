#include "parser.h"

//given input, this class will determine some attributes of input such as pipes, redirects etc
// then the respective handler is called
using namespace std;


int parseLine(string line, vector<string> input) {
    int redir = 0;
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
    }
    if (line.find('<') != string::npos) {
        // redirect in
        cout << "< detected" << endl;
        redir = 1;
    }
    if (line.find('&') != string::npos) {
        // background running
        cout << "& detected" << endl;
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
        redir = 1;
    }

    if (internalHandler(input[0], input) == 0)
        //checks if the internalHandler matched; meaning that an internal command was run and we do not need further execution
        return 0;
    if (redir == 0)
        //
        if(runExt(input)==-1){return -1;};
    if (redir == 1) {
        char out[1024];
        if(runExtRedir(input, out, sizeof(out))==-1){return -1;}
        cout << "REDIR OUT: '\n" << out << "'" << endl;
    }
    return 0;

}