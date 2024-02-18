#include "o2.h"
#include "window.h"
#include <iostream>
using namespace std;
using namespace o2;

string getBuildConfigurationInfo()
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
    cout << "o2-app " << ::getBuildConfigurationInfo() << endl;
    cout << "o2-lib " << o2::getBuildConfigurationInfo() << endl;

    Window window;
    cin.get();
    
    return 0;
}
