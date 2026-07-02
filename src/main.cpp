#include "application.h"
int main()
{
    Application application;

    if(!application.init(800,600, "Title", "Teapot.obj")){
        return 1;
    }
    
    application.run();

    return 0;
}
