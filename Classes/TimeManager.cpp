#include "TimeManager.h"

namespace TimeManager
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    long GetSysTimeSec(){
        timeval now;
        gettimeofday(&now, NULL);
        return now.tv_sec;
    }
    long GetSysTimeMSec(){
        timeval now;
        gettimeofday(&now,NULL);
        return (now.tv_sec*1000+now.tv_usec / 1000);// 小心越界变负值
    }
    long GetSysTimeUSec(){
        timeval now;
        gettimeofday(&now, NULL);
        return (now.tv_sec*1000000 + now.tv_usec);// 小心越界变负值
    }
    string getDate()
    {
        time_t tp = GetSysTimeSec();
        struct tm *tm;
        tm = localtime(&tp);
        char date[32] = { 0 };
        sprintf(date, "%d-%02d-%02d", (int) tm->tm_year + 1900,
                (int) tm->tm_mon + 1, (int) tm->tm_mday);
        return string(date);
    }
#endif
    
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    long GetSysTimeSec(){
        struct tm *tm;
        time_t timep;
        time(&timep);
        tm = localtime(&timep);
        return timep;
    }
    
    string getDate()
    {
        time_t timep;
        time(&timep);
        
        struct tm *tm = localtime(&timep);
        char time[64] = { 0 };
        sprintf(time, "%d-%02d-%02d %02d:%02d:%02d",
                (int) tm->tm_year + 1900,
                (int) tm->tm_mon + 1,
                (int) tm->tm_mday,
                (int) tm->tm_hour,
                (int) tm->tm_min,
                (int) tm->tm_sec);
        
        return string(time);
    }
#endif
    /* 
     C语言,使用基姆拉尔森计算公式,根据日期判断星期几的算法如下：
     基姆拉尔森计算公式：W = (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
     在公式中d表示日期中的日数，m表示月份数，y表示年数。
     注意：在公式中有个与其他公式不同的地方：
     把一月和二月看成是上一年的十三月和十四月，例：如果是2004-1-10则换算成：2003-13-10来代入公式计算。
     以公元元年为参考，公元元年1月1日为星期一</PRE><PRE>程序如下：
     */
    Week CaculateWeekDay(int y,int m, int d)
    {
        if(m==1||m==2) {
            m += 12;
            --y;
        }
        
        int iWeek = ( d + 2*m + 3*(m+1)/5 + y + y/4 - y/100 + y/400) % 7;
        switch(iWeek)
        {
            case 0: cout <<"星期一"<<endl; break;
            case 1: cout <<"星期二"<<endl; break;
            case 2: cout <<"星期三"<<endl; break;
            case 3: cout <<"星期四"<<endl; break;
            case 4: cout <<"星期五"<<endl; break;
            case 5: cout <<"星期六"<<endl; break;
            case 6: cout <<"星期日"<<endl; break;
        }
        
        return static_cast<Week>(iWeek);
    }
}