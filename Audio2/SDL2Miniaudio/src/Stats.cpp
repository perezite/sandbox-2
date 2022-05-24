#include "Stats.h"

#include "Stopwatch.h"
#include "Logger.h"
#include "Settings.h"

using namespace sb;

namespace blocks 
{
    void printFramerate()
    {
        static Stopwatch stopwatch;
        static size_t counter = 0;
        counter++;

        if (counter == 100) {
            float elapsedMs = stopwatch.getElapsedMs();
            stopwatch.reset();
            float framerate = (1000.0f * counter) / elapsedMs;
            counter = 0;
            SB_MESSAGE("fps: " << int(framerate));
        }
    }

    void printStats()
    {
        #if BLOCKS_PRINT_FRAMERATE
            printFramerate();
        #endif
    }
}
