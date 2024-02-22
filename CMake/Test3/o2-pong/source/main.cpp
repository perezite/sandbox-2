#include "lib.h"
#include <iostream>
using namespace std;
using namespace lib;

namespace app
{
    string getBuildConfigurationInfo()
    {
        #ifdef _DEBUG
            return "debug build";
        #endif
    
        #ifdef NDEBUG
            return "release build";
        #endif
    }
}

int main()
{
    cout << "o2-app " << app::getBuildConfigurationInfo() << endl;
    cout << "lib " << lib::getBuildConfigurationInfo() << endl;

    cin.get();

    return 0;
}