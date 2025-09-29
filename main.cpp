#include "linker.h"

int main()
{
    std::cout<<"start!"<<std::endl;
    Linker lk;
    lk.init();
    std::cout<<"end!"<<std::endl;
    getchar();
    lk.exit();
}