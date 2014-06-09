#ifndef __GameTest__TimeManager__
#define __GameTest__TimeManager__

#include <iostream>
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

namespace TimeManager {
    
    enum Week{
        eSunday,
        eMonday,
        eTuesDay,
        eWednesDay,
        eThursDay,
        eFriday,
        eSaturday
    };
    
    // 1s=1000ms(毫秒)=1000000us(微秒)
    long GetSysTimeSec();   //取系统当前时间(单位为秒)
    string getDate();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    long GetSysTimeMSec();  //取系统当前时间(单位为毫秒)
    long GetSysTimeUSec();  //取系统当前时间(单位为微秒)
#endif
    Week CaculateWeekDay(int y,int m, int d);
}


#endif
