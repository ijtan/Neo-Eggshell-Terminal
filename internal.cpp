#include "internal.h"

using namespace std;
vector<string> internalCommands;
int internalHandler(string command){
    //will be used to check if the command is internal
    internalCommands.emplace_back("echo");

    for(auto & internalCommand : internalCommands) {
        if(internalCommand==(command)){
            cout << "Internal command found: " <<command<<endl;
            switch(&internalCommand-&internalCommands[0]){
                case 0:
                    cout << "echo has been reached!"<<endl;
                    break;
            }
            return 0;
        }
    }return 1;
}