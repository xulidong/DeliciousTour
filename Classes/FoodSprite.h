/*
 一 位置
 
 二 食物种类
 
 二 状态
 1 横消
 2 竖消
 3 爆炸
 4 同色消
 
 三 生命值
 1 普通为1
 2 被束缚的为2
 3 冰块 N
 4 厨具未-1
 
 *为0的将会被消除
 */

#ifndef __FoodSprite_H__
#define __FoodSprite_H__

#include "cocos2d.h"

USING_NS_CC;

enum class FoodState{
    FOOD_STATE_NONE,
    FOOD_STATE_NORMAL = FOOD_STATE_NONE,
    FOOD_STATE_HORIZONTAL,
    FOOD_STATE_VERTICAL,
    FOOD_STATE_EXPLODE,
    FOOD_STATE_SAME,
    FOOD_STATE_BRING,
    FOOD_STATE_MAX
} ;

enum class FoodType{
    FOOD_TYPE_NONE,
    FOOD_TYPE_1 = FOOD_TYPE_NONE,
    FOOD_TYPE_2,
    FOOD_TYPE_3,
    FOOD_TYPE_4,
    FOOD_TYPE_5,
    FOOD_TYPE_6,
    FOOD_TYPE_MAX
};

class FoodSprite :  public Sprite
{
public:
    FoodSprite();
	static FoodSprite *create(FoodType type,int row, int col);
    static float getContentWidth();
    static float getContentHeight();
    
    CC_SYNTHESIZE(int, m_row, Row);
    CC_SYNTHESIZE(int, m_col, Col);
    
    CC_SYNTHESIZE(FoodType, m_foodType, FoodType);
    
    CC_SYNTHESIZE_READONLY(FoodState, m_foodState, FoodState);
    void setFoodState(FoodState mode);
    
    CC_SYNTHESIZE_READONLY(int, m_life, Life);
    void setLife(int life);
    void removeOneLife(int count = 1);
    
    // 标记是否可以移除一次，移除一次生命值减一
    CC_SYNTHESIZE(bool, m_isNeedRemove, IsNeedRemove);
    
    //新产生的4消寿司，本轮不被消除
    CC_SYNTHESIZE(bool, m_ignoreCheck, IgnoreCheck);
};

#endif
