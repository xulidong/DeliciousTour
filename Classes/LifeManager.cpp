#include "LifeManager.h"
#include "TimeManager.h"

#define GAME_LIFE_TIME "gamelifetime"
#define LIFE_TIME_DISTANCE (60 * 3)

LifeManager::LifeManager(int lifeLimit)
: m_life(0)
, m_lifeLimit(lifeLimit)
{
    
}

LifeManager::~LifeManager()
{
    
}

bool LifeManager::addLife( int life)
{
    if (life < 0 && m_life == m_lifeLimit) {
        setLifeTime();
    }
    
    if (m_life + life > m_lifeLimit || m_life + life <0) {
        return false;
    }
    
    m_life += life;
    
    return true;
}

long LifeManager::getLifeTime()
{
    return UserDefault::getInstance()->getIntegerForKey(GAME_LIFE_TIME, 0);
}

void LifeManager::setLifeTime(long time)
{
    UserDefault::getInstance()->setIntegerForKey(GAME_LIFE_TIME,time);
    UserDefault::getInstance()->flush();
}

void LifeManager::setLife(int life)
{
    m_life = life;
}

int LifeManager::getLife()
{
    updateLife();// 更新生命值
    
    return m_life;
}

long LifeManager::getNextLifeTime()
{
    updateLife();// 更新生命值
    if (m_life != m_lifeLimit) {
        int time = LIFE_TIME_DISTANCE - ( TimeManager::GetSysTimeSec()-getLifeTime() )%(int)LIFE_TIME_DISTANCE;
        return time;
    }
    return -1;
}

void LifeManager::updateLife()
{
    long timeDistance = TimeManager::GetSysTimeSec()-getLifeTime();
    if (timeDistance > LIFE_TIME_DISTANCE) {
        int addition = (timeDistance)/LIFE_TIME_DISTANCE;
        m_life += addition;
        m_life = MIN(m_life, m_lifeLimit);
        setLifeTime(getLifeTime()+addition*LIFE_TIME_DISTANCE);
    }
}

void LifeManager::setLifeLimit(int lifeLimit)
{
    m_lifeLimit = lifeLimit;
}
