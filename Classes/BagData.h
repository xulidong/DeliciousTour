
#ifndef __GameTest__BagData__
#define __GameTest__BagData__

#include <iostream>
#include "CommonHeader.h"
#include "Props.h"
#include "LifeManager.h"

enum eBagLevel{
    eBagLevel_None,
    eBagLevel_1 = 0,
    eBagLevel_2,
    eBagLevel_3,
    eBagLevel_4,
    eBagLevel_5,
    eBagLevel_Max
};

class BagData : public Ref
{
    struct BagLimit{
        int level;
        int life;
        int coins;
        int props;
    };
    
public:
    BagData();
    ~BagData();
    
    // **************************** 当前值 *************************************
    CC_SYNTHESIZE_READONLY(int, m_bagLevel, BagLevel);
    void setBagLevel(eBagLevel bagLevel);
    
    CC_SYNTHESIZE_READONLY(int, m_coins, Coins);
    void setCoins(int coins);
    bool addCoins(int coins);

    // 设置道具的数量
    void setPropsCountOf(int count, eProps props);
    
    // 会在PlayerData中赋值，为了方便，定义为public
    Props m_props[ePropsMax];
    
    LifeManager* getLifeManager();
    
private:
    int m_propsCount;
    void reFreshPropsCount();
    bool canShowMoreProps();
    
    BagLimit m_bagLimit[eBagLevel_Max];
    LifeManager* m_lifeManager;
};

#endif 
