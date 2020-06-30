#include "parser.h"

#include "reparse.h"

// given input, this class will determine some attributes of input such as
// pipes, redirects etc
// then the respective handler is called
using namespace std;
#define MAX_ARGS 100

//INDEX: this is useful to unserstand flags raised by the flagger()
// 0: >>
// 1: >
// 2: <
// 3: |
// 4: & -> background run

int parseLine(string line, vector<string> input) {
    vector<int> RedirectConfig(5); //start the config vector

    if (line.find(';') != string::npos) {  //if multi command
        if (multiCommands(line, input) == -1) 
            return 0;
    }
    flagger(line, RedirectConfig);

    if (line.find('"') != string::npos) { 
        // quotes for combining args
        if (better_String_Combiner(input, line) == -1) {
            return 0;
        };
    }
    if (line.find('$') != string::npos) {
        // if var exists replace the word with the value and return value
        parseVars(input, line);
    }
    if (line.find('=') != string::npos) {
        if (input.size() == 1) {
            setenv(line.substr(0, line.find('=')).c_str(),
                   line.substr(line.find('=') + 1).c_str(), 1);
        }
        return 0;
    }

    if (line.find('&') != string::npos) { //amperdand -> background run
        // background running
        if (input[input.size() - 1] == "&" && input[input.size() - 1].find('&') != string::npos) {
            input.erase(input.end());
            line.erase(line.find('&'));
            RedirectConfig[4] = 1;
        }
    }

    //execute -> acts upon classfiication
    return Executor(input, line, RedirectConfig);
}

int reParse(string line, vector<string> &input) { //this calls the porser, used to avoid header loop. 
    return parseLine(move(line), move(input));
}
int tokenize(char *line, char *copy, vector<string> &args) { //splits a char* into an array of chars. delimited by spaces
    char *token = NULL;
    int tokenIndex;
    if (strlen(line) == 0) {
        free(line);
        args.clear();
        return -1;
    }
    token = strtok((char *)copy, " ");
    for (tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1;
         tokenIndex++) {
        args.push_back(token);
        token = strtok(NULL, " ");
    }
    return 0;
}
int better_String_Combiner(vector<string> &input, string &line) {  // counting to see if odd or even number of ""
    int i = 0, count = 0;
    for (const auto &arg : input) {
        if (arg.find('\"') != string::npos)
            for (char i : arg)
                if (i == '\"')
                    count++;
    }
    // would mean that we could not find the ""
    if (count == 0) {
        cerr << "Quote handling error" << endl;
    }
    string more;  // this string will hold newly inputted characters, will be
    // appended to iriginal command
    // checking if divisible by 2 > odd / even
    if (count % 2 != 0) {
        // if odd -> keep taking whole lines until the number of " is even, by
        // taking whole lines we can allow the user to: [mkdir "file one" "file
        // two"]
        cout << "+>";
        char buf = '\0';

        int esc = 0;
        while (count % 2 != 0 || esc == 0) {
            esc = 0;
            buf = getc(stdin);
            if (buf == '\n') {
                cout << "+>";
                esc = 1;
                buf = ' ';
            }
            if (buf == '\"')
                count++;
            more.push_back(buf);  // add current char to vector
        }
        // add more to the input list, this will be
        // handled and removed later in the function
        input.push_back(more);
        line.append(more);  // apennd the newly added characters to the line
    }
    //for each couple of quotes
    //starts actually combining
    for (int i = 0; i < count / 2; ++i) {
        //init vars
        int startArg, startPos, endArg, endPos;
        startArg = startPos = endArg = endPos = -1;
        //this find the index of argument which has the starting quote
        //(and index of char in string which as the quote)
        for (int j = 0; j < input.size(); j++) {
            if (input[j].find('\"') != string::npos) {
                startArg = j;
                startPos = input[j].find('\"');
                break;
            }
        }
        //find pos of argument with ending quote (and index of char in string which as the quote)
        for (int j = 0; j < input.size(); j++) {
            if (input[j].find('\"') != string::npos) {
                int found = 0;
                if (startArg == j) {
                    for (endPos = startPos + 1; endPos < input[j].length(); endPos++)
                        if (input[j][endPos] == '\"') {
                            found = 1;
                            break;
                        }
                    if (found == 0)
                        continue;

                } else
                    endPos = input[j].find('\"');
                endArg = j;
                break;
            }
        }
        //if we do not find a quote, the start or endarg and not changed from the -1 they started with ->  and thus we alert the user accoridngly 
        if (startArg > endArg || startArg == -1 || endArg == -1) {
            cerr << "Could not Parse Quotes. Aborting..." << endl;
            return -1;
        }
        string combinedArg;
        if (startArg != endArg) {
            if (input[startArg].length() != 1 || startPos != 0) { //if in the argument there isnt just a '"' we add the contents of the argument toi the combined arg which comes afte ther quote
                combinedArg.append(input[startArg].substr(startPos + 1)); 
                combinedArg.append(" ");
            }
            for (int j = startArg + 1; j < endArg; j++) { //we append all arguments which come after the start quote (and before the end quote) into the combinedArg string.
                combinedArg.append(input[j]);
                combinedArg.append(" ");
            }
            if (input[endArg].length() != 1 || endPos != 0) //if there is text in the same argument as the end quote, we add the contents of the arg to the combined arg
                combinedArg.append(input[endArg].substr(0, endPos));
        } else { //this does the same as the latter 3 steps, yet applied to when the quotes are in the same argument element. (Thus we work only with one string rather than a whole array of them)
            string betweenQuotes = input[endArg].substr(startPos + 1, endPos - startPos - 1);
            if (betweenQuotes.length() != 0)
                combinedArg.append(betweenQuotes);
        }
        ////we add the text which comes after the quote into a seperate element (since startArg one will be removed)
        if (!input[startArg].substr(0, startPos).empty()) {
            input.insert(input.begin() + startArg, input[startArg].substr(0, startPos));
            startArg++;
            endArg++;
        }
        if (!input[endArg].substr(endPos + 1).empty()) { // we add the contents of the argument which comes after the quotes to a seperate element
                  input.insert(input.begin() + endArg + 1, input[endArg].substr(endPos + 1));
        }
        input.erase(input.begin() + startArg, input.begin() + endArg + 1); //remove all elements between and including the start and end args
        input.insert(input.begin() + startArg, combinedArg); //insert the combined arg in the old posiiotn of the starting quote
    }//if there are any quotes left, restart loop

    line.clear(); //remove content of line
    for (auto arg : input) { // re create the 'line' steing to be according to the new arg 
        line.append(arg);
        line.append(" ");
    }
    return 0;
}
int multiCommands(string &line, vector<string> &input) {
    string newLine = line.substr(0, line.find_last_of(';'));  //create new string without the first command
    line = line.substr(line.find_last_of(';') + 1);           //make command for current execution without others

    vector<string> preArgs;

    for (int argPos = input.size() - 1; argPos >= 0; --argPos) {
        string arg = input[argPos];
        if (arg.find(';') != string::npos) {  //find which arg has the semicolon

            for (int cnt = 0; cnt < argPos; ++cnt)  //add all other args
                preArgs.push_back(input[cnt]);

            if ((arg.substr(0, arg.find_last_of(';'))).length() != 0)     //&& arg.length() != arg.find(';') + 1)       //if arg is simply a semicolon or semicolon is at the end skip it!
                preArgs.push_back(arg.substr(0, arg.find_last_of(';')));  //add text post-semicolon

            if ((arg.substr(arg.find_last_of(';') + 1)).length() != 0)
                input.insert(input.begin() + argPos + 1, arg.substr(arg.find_last_of(';') + 1));  //else do some magic to copy text pre-colon
            input.erase(input.begin(), input.begin() + argPos + 1);                               //remove post-colon-contining-arg args

            break;
        }
    }
    //several error checks
    if (preArgs.empty() || input.empty()) {
        cerr << "Bad Semicolon formation found, Aborting..." << endl;
        return -1;
    }
    for (auto arg : preArgs) {
        if (arg.length() == 0) {
            cerr << "Bad Semicolon formation found, Aborting..." << endl;
            return -1;
        }
    }
    for (auto arg : input) {
        if (arg.length() == 0) {
            cerr << "Bad Semicolon formation found, Aborting..." << endl;
            return -1;
        }
    }
    //parse the new sub-part of the input
    parseLine(newLine, preArgs);
    return 0;
}