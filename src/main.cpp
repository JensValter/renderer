#include "application.h"
int main()
{
    Application application;

    if(!application.init(960,540, "Title", "Teapot.obj")){
        return 1;
    }
    
    application.run();

    return 0;
}
