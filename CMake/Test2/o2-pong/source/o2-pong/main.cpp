#include "window.h"
#include <iostream>
using namespace std;
using namespace o2;

string getBuildTypeInfo()
{
    #ifdef _DEBUG
        return "debug build";
    #endif
    
    #ifdef NDEBUG
        return "release build";
    #endif
}

int main()
{
    cout << "o2-app " << getBuildTypeInfo() << endl;

    Window window;
    cin.get();
    
    return 0;
}
