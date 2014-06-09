#ifndef __GameTest__LifeManager__
#define __GameTest__LifeManager__

#include <iostream>
#include "CommonHeader.h"

class LifeManager
{
public:
    ~LifeManager();
    LifeManager(int lifeLimit);
    
    bool addLife(int life = 1);
    void setLife(int life);
    int getLife();
    
    // 返回值：－1-生命值为满
    long getNextLifeTime();
    
    void updateLife();
    void setLifeLimit(int lifeLimit);
private:
    int m_life;
    int m_lifeLimit;
    
    long getLifeTime();
    void setLifeTime(long time = TimeManager::GetSysTimeSec() );
};

#endif
