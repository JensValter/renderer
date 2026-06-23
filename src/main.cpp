#include "application.h"
#include <iostream>
int main()
{
    Application application;

    std::cout<<"test"<<'\n';

    if(!application.init(800,600, "Title", "Teapot.obj")){
        return 1;
    }
    application.run();

    return 0;
}