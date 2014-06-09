/*
 1 解锁关卡
 2 背包等级
 3 包子数
 4 闪砖数
 5 道具数
 */

#ifndef __GameTest__PlayerData__
#define __GameTest__PlayerData__

#include <iostream>
#include "CommonHeader.h"
#include "BagData.h"

class PlayerData : public Ref
{
public:
    ~PlayerData();
    static PlayerData* getInstance();
    static void destroyInstance();
    
    void readFromFile();
    void savaToFile();
    
    // 最大解锁关卡
    CC_SYNTHESIZE(int, m_maxUnlockLevel, MaxUnlockLevel);
    
    BagData* getBagData();
private:
    PlayerData();
    static PlayerData* data;
    
    BagData* m_bagData;
};

#endif
