#include "parser.h"
#include "reparse.h"

//given input, this class will determine some attributes of input such as pipes, redirects etc
// then the respective handler is called
using namespace std;
#define MAX_ARGS 100

int tokenize(char *line, char *copy, vector<string> &args) {
    char *token = NULL;
    int tokenIndex;
    if (strlen(line) == 0) {
        free(line);
        args.clear();
        return -1;
    }
    token = strtok((char *) copy, " ");
    for (tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1; tokenIndex++) {
        args.emplace_back(token);
        token = strtok(NULL, " ");
    }
    return 0;
}

int parseLine(string line, vector<string> input) {
    int RedirectConfig[5] = {0};
    //0: >>
    //1: >
    //2: <
    //3: |
    //4: & -> background run

    if (line.find('=') != string::npos) {
        if (input.size() == 1) {
            if (set(input) == 0)
                return 0;
        }
    }
    if (line.find(">>") != string::npos)
        RedirectConfig[0] = 1;
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
        if (input[input.size() - 1] == "&") {
            input.erase(input.end());
            cout << "& detected" << endl;
            RedirectConfig[4] = 1;
        }

    }
    if (line.find('"') != string::npos) {
        // quotes for combining args

        int i = 0, count = 0;
        for (const auto &arg:input) {
            if (arg.find('\"') != string::npos)
                for (char i : arg)
                    if (i == '\"')
                        count++;
        }
        if (count == 0) {
            cerr << "\" error" << endl;
        }
        string more;
        if (count % 2 != 0) {
            cout << "+>";
            char buf = '\0';

            //cin.rdbuf()->in_avail() is supposed to check if stdin is empty
            //https://tinyurl.com/yby36dak
            int esc = 0;
            while (count % 2 != 0 || esc == 0) {
                esc = 0;
                more.push_back(buf);
                buf = getc(stdin);
                if (buf == '\n') {
                    cout << "+>";
                    esc = 1;
                    buf = ' ';
                }
                if (buf == '\"')
                    count++;
            }
            more.push_back(buf);
            input.push_back(more);
            line.append(more);
        }
        while (count > 0) {
            int posA = 0, posB = 0;
            int startArg, endArg;
            startArg = endArg = -1;
            for (auto arg:input) {
                posA = arg.find('\"');
                if (posA == 0) {
                    startArg = i;
                    break;
                }
                i++;
            }
            i = 0;
            for (auto arg:input) {
                posB = arg.find('\"');
                if (posB == arg.size() - 1) {
                    endArg = i;
                    break;
                }
                i++;
            }
            string newStr;
            if (startArg != -1 && endArg != -1 && startArg <= endArg) {
                input[startArg] = input[startArg].substr(1);
                input[endArg] = input[endArg].substr(0, input[endArg].size() - 1);
                for (int i = startArg; i <= endArg; i++) {
                    newStr.append(input[i]);
                }
                input.erase(input.begin() + startArg, input.begin() + endArg + 1);
                input.push_back(newStr);
                count -= 2;
            } else
                break;
        }
        for (auto in:input)
            cout << "[" << in << "]" << endl;
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
    if (runExt(input, RedirectConfig) == -1) { return -1; };
    return 0;

}

int reParse(string line, vector<string> &input) {
    return parseLine(move(line), move(input));
}