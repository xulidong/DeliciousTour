#ifndef __DeliciousTour__BaseFood__
#define __DeliciousTour__BaseFood__

#include "cocos2d.h"

USING_NS_CC;

enum class FoodType{
    FOOD_TYPE_NONE,
    FOOD_TYPE_1 = FOOD_TYPE_NONE,
    FOOD_TYPE_2,
    FOOD_TYPE_3,
    FOOD_TYPE_4,
    FOOD_TYPE_5,
    FOOD_TYPE_6,
    FOOD_TYPE_SAME,
    FOOD_TYPE_BRING,
    FOOD_TYPE_MAX
};

class BaseFood : public Sprite
{
public:
    BaseFood();
    virtual ~BaseFood();
    bool init(FoodType type,int row, int col);
	static BaseFood *create(FoodType type,int row, int col);
    
    static float getContentWidth();
    static float getContentHeight();
    
    CC_SYNTHESIZE(int, m_row, Row);
    CC_SYNTHESIZE(int, m_col, Col);
    
    CC_PROPERTY(FoodType, m_foodType, FoodType);
    
    CC_SYNTHESIZE_READONLY(int, m_life, Life);
    void setLife(int life);
    void removeLife(int count = 1);
    
    // 标记是否可以移除一次，移除一次生命值减一
    CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);
    
    //新产生的4消寿司，本轮不被消除
    CC_SYNTHESIZE(bool, m_ignoreCheck, IgnoreCheck);
};

#endif 
