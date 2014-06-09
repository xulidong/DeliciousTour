
#include "BagData.h"
#include "LifeManager.h"

BagData::BagData()
: m_bagLevel(eBagLevel_None)
, m_coins(0)
, m_propsCount(0)
{
    // 读取背包等级配置文件
    ValueVector bagfile_vec = FileUtils::getInstance()->getValueVectorFromFile(BAG_CONGIG_FILE);
    for( int i = 0 ; i < bagfile_vec.size() && i < eBagLevel_Max; ++i)
    {
        auto e = bagfile_vec.at(i);
        auto item_vec = e.asValueVector();
        m_bagLimit[i].level = item_vec.at(0).asInt();
        m_bagLimit[i].life = item_vec.at(1).asInt();
        m_bagLimit[i].coins = item_vec.at(2).asInt();
        m_bagLimit[i].props = item_vec.at(3).asInt();
    }
    
    // 读取道具配置文件道具(道具不可改变的属性放在配置文件中)
    ValueVector propsfile_vec = FileUtils::getInstance()->getValueVectorFromFile(Props_CONFIG_FILE);
    for (int i = 0; i < propsfile_vec.size() && i < ePropsMax; ++i)
    {
        auto e = propsfile_vec.at(i);
        auto item_vec = e.asValueVector();
        m_props[i].setId(static_cast<eProps>(item_vec.at(0).asInt()));
        m_props[i].setName(item_vec.at(1).asString());
        m_props[i].setDes(item_vec.at(2).asString());
        
        // 以下两个值是动态的，将会在PlayerData中读取，此处只是赋个初始值
        m_props[i].setCount(0);
        m_props[i].setVisible(false);
    }
    
    m_lifeManager= new LifeManager(m_bagLimit[m_bagLevel].life);
}

BagData::~BagData()
{
    delete m_lifeManager;
}

void BagData::setCoins(int coins)
{
    m_coins = coins;
}

bool BagData::addCoins(int coins)
{
    if (m_coins + coins > m_bagLimit[m_bagLevel].coins) {
        return false;
    }
    m_coins += coins;
    
    return true;
}

void BagData::reFreshPropsCount()
{
    for (int i = 0; i < ePropsMax; ++i) {
        if(m_props[i].getVisible()){
            ++m_propsCount;
        }
    }
}

bool BagData::canShowMoreProps()
{
    return (m_propsCount < m_bagLimit[m_bagLevel].props);
}

void BagData::setPropsCountOf(int count, eProps props)
{
    m_props[props].setCount(count);
    m_props[props].setVisible(canShowMoreProps());
    reFreshPropsCount();
}

LifeManager* BagData::getLifeManager()
{
    // 因为背包等级有可能已经升级，所以在获取之前要重新设置一下生命限制
    m_lifeManager->setLifeLimit(m_bagLimit[m_bagLevel].life);
    return m_lifeManager;
}

void BagData::setBagLevel(eBagLevel bagLevel)
{
    m_bagLevel = bagLevel;
    // 背包等级改变，随之改变生命上限
    m_lifeManager->setLifeLimit(m_bagLimit[m_bagLevel].life);
}