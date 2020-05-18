#include <iostream>
#include "linenoise.h"

#define MaxCmdLen 100
int main()
{
    std::cout<<"Welcome to EggShell!"<<std::endl;
    char Input[MaxCmdLen];
    std::cin >> Input;

    return 0;
}
