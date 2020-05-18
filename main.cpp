#include <iostream>
#include "linenoise.h"
#include "external.h"

#define MaxCmdLen 100
int main()
{
    std::cout<<"Welcome to EggShell!"<<std::endl;
    char Input[MaxCmdLen];
    std::cin >> Input;
    runExt(Input);
    return 0;
}
