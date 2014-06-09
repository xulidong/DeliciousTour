#include "CommonFunction.h"

#include "TimeManager.h"

namespace CommonFunction
{
    int getRandNum(int from, int to)
    {
        unsigned long int randSeed = TimeManager::GetSysTimeMSec();
        srand(randSeed);
        
        float num = CCRANDOM_0_1()*(to - from + 1) + from;
        return static_cast<int>(num);
    }
}