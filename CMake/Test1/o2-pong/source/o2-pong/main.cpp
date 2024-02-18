#include "window.h"
#include <iostream>
using namespace std;
using namespace o2;

string getDebugModeInfo()
{
    #ifdef _DEBUG
        return "debug mode";
    #endif
    
    #ifdef NDEBUG
        return "release mode";
    #endif
}

int main()
{
    cout << "o2-app " << getDebugModeInfo() << endl;

    Window window;
    cin.get();
    
    return 0;
}
