#include "parser.h"

#include "reparse.h"

// given input, this class will determine some attributes of input such as
// pipes, redirects etc
// then the respective handler is called
using namespace std;
#define MAX_ARGS 100

int tokenize(char *line, char *copy, vector<string> &args)
{
    char *token = NULL;
    int tokenIndex;
    if (strlen(line) == 0)
    {
        free(line);
        args.clear();
        return -1;
    }
    token = strtok((char *)copy, " ");
    for (tokenIndex = 0; token != NULL && tokenIndex < MAX_ARGS - 1;
         tokenIndex++)
    {
        args.push_back(token);
        token = strtok(NULL, " ");
    }
    return 0;
}

int parseLine(string line, vector<string> input)
{
    vector<int> RedirectConfig(5);
    // 0: >>
    // 1: >
    // 2: <
    // 3: |
    // 4: & -> background run

    if (line.find(';') != string::npos)
    {
        //line erase from - until pos of ';'
        //argVec erase until the pos of command containing;
        //remove ; from the found arg from argvec
        //check if arg becomes empty after remova meaning the user left space before ';'
        //^or just check if length = 1 before removal

        //reparse //// nooo not reparse just call self -> true recursion
    }
    flagger(line, RedirectConfig);

    if (line.find('"') != string::npos)
    {
        // quotes for combining args
        stringCombiner(input, line);
        cout << "afta" << endl;
        int ccount = 0;
        for (auto n : input)
        {
            cout << ccount << "=[" << n << ']' << endl;
            ccount++;
        }
    }

    if (line.find('=') != string::npos)
    {
        if (input.size() == 1)
        {
            better_set(line.substr(0, line.find('=')),
                       line.substr(line.find('=') + 1, line.size()));
        }
        return 0;
    }
    if (line.find('$') != string::npos)
    {
        // if var exists replace the word with the value and return value
        input = parseVars(input);
    }
    if (line.find('&') != string::npos)
    {
        // background running
        if (input[input.size() - 1] == "&")
        {
            input.erase(input.end());
            cout << "& detected" << endl;
            RedirectConfig[4] = 1;
        }
    }

    //execute
    return Executor(input, RedirectConfig);
}

int reParse(string line, vector<string> &input)
{
    return parseLine(move(line), move(input));
}

void stringCombiner(vector<string> &input, string &line)
{
    // counting to see if odd or even number of ""
    int i = 0, count = 0;
    for (const auto &arg : input)
    {
        if (arg.find('\"') != string::npos)
            for (char i : arg)
                if (i == '\"')
                    count++;
    }
    // would mean that we could not find the ""
    if (count == 0)
    {
        cerr << "\" error" << endl;
    }
    string more; // this string will hold newly inputted characters, will be
    // appended to iriginal command
    // checking if divisible by 2 > odd / even
    if (count % 2 != 0)
    {
        // if odd -> keep taking whole lines until the number of " is even, by
        // taking whole lines we can allow the user to: [mkdir "file one" "file
        // two"]
        cout << "+>";
        char buf = '\0';

        int esc = 0;
        while (count % 2 != 0 || esc == 0)
        {
            esc = 0;
            buf = getc(stdin);
            if (buf == '\n')
            {
                cout << "+>";
                esc = 1;
                buf = ' ';
            }
            if (buf == '\"')
                count++;
            more.push_back(buf); // add current char to vector
        }
        cout << endl;
        // add more to the input list, this will be
        // handled and removed later in the function
        input.push_back(more);
        line.append(more); // apennd the newly added characters to the line
    }
    // some logic which puts the content between the "" in a single args element
    // rathe than seperated by ' ' as theyw ere before
    while (count > 0)
    {
        int startArg, endArg;
        startArg = endArg = -1;
        int i = 0;
        // checking each argument to see if ti starts with a "
        for (auto arg : input)
        {
            if (arg.find('\"') != string::npos)
            {
                startArg = i;
                break;
            }
            i++;
        }
        int j = 0;
        // checking each argument to see if to end with a " -> posA and posB would
        // be the limits of the new arg since a is starting " and B is closing "
        for (auto arg : input)
        {
            if (arg.find('\"') != string::npos && j != startArg)
            {
                endArg = j;
                break;
            }
            j++;
        }
        string newStr = "";
        if (startArg != -1 && endArg != -1 &&
            startArg <= endArg)
        {                                                                             // checks that positions found are valid
            input[startArg] = input[startArg].substr(input[startArg].find('\"') + 1); // removes the starting "
            input[endArg].erase(input[endArg].find('\"'), 1);                         // removes the end "

            // append all elements from input[], starting from the to one string,
            // which will be a new one whole argument with all 'subargs'
            for (int i = startArg; i <= endArg && input[i].find('\"') == string::npos; i++)
            {
                newStr.append(input[i]);

                if ((input[i].find(' ') ==
                     string::npos) &&
                    i < endArg) // this chekc if the input has any spaces, if it
                    // doesnt, it would mean that it was given as
                    // original arg and not added. (ex: called [mkdir
                    // "file A"]);  since tokenization splits by space
                    // we would get [mkdir]["file][A"] -> when appended
                    // [mkdir fileA] -> gets rid of space -> we add it
                    // back here
                    newStr.append(" "); //
            }
            // remove the args which have been converted
            input.erase(input.begin() + startArg, input.begin() + endArg + 1);
            input.insert(input.begin() + startArg, newStr);
            count -= 2;
        }
        else
            break;
    }
}