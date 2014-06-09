#include "PlayerData.h"

#define PlAYER_DATA_FILE    "PlayDataFile"

#define kMaxUnlockLevel     "kMaxUnlockLevel"
#define kBagLevel           "kBagLevel"
#define kLife               "kLife"
#define kCoins              "kCoins"

const static string kProps[ePropsMax] = {
    "kPropsLine",
    "kPropsExplode",
    "kPropsSame",
    "kPropsStorm",
    "kPropsSwap",
    "kPropsClock",
    "kPropsChilli",
    "kPropsSort",
    "kPropsBoom"
};

PlayerData::PlayerData()
: m_maxUnlockLevel(1)
{
    m_bagData = new BagData();
}

PlayerData::~PlayerData()
{
    delete m_bagData;
}

void PlayerData::readFromFile()
{
    m_maxUnlockLevel = UserDefault::getInstance()->getIntegerForKey(kMaxUnlockLevel, 1);
    // 背包
    m_bagData->setBagLevel( static_cast<eBagLevel>(UserDefault::getInstance()->getIntegerForKey(kBagLevel, 1)) );
    m_bagData->getLifeManager()->setLife( UserDefault::getInstance()->getIntegerForKey(kLife, 5) );
    m_bagData->setCoins( UserDefault::getInstance()->getIntegerForKey(kCoins, 500) );
    // 道具(道具有两个属性需要保存，为了方便，用一个数字表示，最后一位表示“是否显示”，其他表示“数量”)
    for (int i = ePropsNone; i < ePropsMax; ++i) {
        int value = UserDefault::getInstance()->getIntegerForKey(kProps[i].c_str(),0);
        m_bagData->m_props[i].setCount(value/10);
        m_bagData->m_props[i].setVisible(value%10);
    }
}

PlayerData* PlayerData::data = NULL;

PlayerData* PlayerData::getInstance()
{
    if (!data) {
        data = new PlayerData();
    }
    
    return data;
}

void PlayerData::destroyInstance()
{
    CC_SAFE_DELETE(data);
}

BagData* PlayerData::getBagData()
{
    return m_bagData;
}

void PlayerData::savaToFile()
{
    UserDefault::getInstance()->setIntegerForKey(kMaxUnlockLevel, m_maxUnlockLevel);
    // 背包
    UserDefault::getInstance()->setIntegerForKey(kBagLevel, m_bagData->getBagLevel());
    UserDefault::getInstance()->setIntegerForKey(kLife, m_bagData->getLifeManager()->getLife());
    UserDefault::getInstance()->setIntegerForKey(kCoins, m_bagData->getCoins());
    // 道具(道具有两个属性需要保存，为了方便，用一个数字表示，最后一位表示“是否显示”，其他表示“数量”)
    for (int i = ePropsNone; i < ePropsMax; ++i) {
        int value = m_bagData->m_props[i].getCount()*10+m_bagData->m_props[i].getVisible();
        UserDefault::getInstance()->setIntegerForKey(kProps[i].c_str(),value);
    }
    
    UserDefault::getInstance()->flush();
}