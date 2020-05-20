#include "internal.h"

using namespace std;
vector<string> internalCommands;
int internalHandler(string command, char** args){
    //will be used to check if the command is internal
    internalCommands.emplace_back("echo");

    for(auto & internalCommand : internalCommands) {
        if(internalCommand==(command)){
            cout << "Internal command found: " <<command<<endl;
            switch(&internalCommand-&internalCommands[0]){
                case 0:
                    echo(args);
                    break;
            }
            return 0;
        }
    }return 1;
}
void echo(char **args){
    int i = 1;
    while(args[i]!=NULL) {
        cout << args[i] << " ";
        i++;
    }
    cout<<endl;
}