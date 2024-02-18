#include "o2.h"

namespace o2
{
    std::string getBuildConfigurationInfo()
    {
        #ifdef _DEBUG
            return "debug build";
        #endif

        #ifdef NDEBUG
            return "release build";
        #endif 
    }
}
